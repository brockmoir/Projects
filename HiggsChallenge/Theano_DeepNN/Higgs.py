import csv
import math
import os
import sys
import time
import cPickle

import numpy as np

from sklearn import preprocessing
from sklearn.cross_validation import train_test_split
from sklearn.cluster import MiniBatchKMeans

import theano
import theano.tensor as T
from theano.tensor.shared_randomstreams import RandomStreams

from logistic_sgd import LogisticRegression
from mlp import HiddenLayer
from dA import dA
from SdA import SdA

theano.config.openmp = True

#################### Helpful functions ########################
###############################################################

def AMS(y,p,w):
    
    """ Approximate Median Significance defined as:
        AMS = sqrt(
        2 { (s + b + b_r) log[1 + (s/(b+b_r))] - s}
        where b_r = 10, b = background, s = signal, log is natural logarithm """
    
    br = 10.0
    s = (y*p*w).sum()
    b = ((1-y)*p*w).sum()
    radicand = 2 *( (s+b+br) * math.log (1.0 + s/(b+br)) -s)
    if radicand < 0:
        print 'radicand is negative. Exiting'
        exit()
    else:
        return math.sqrt(radicand)

def rank(a):
    
    """ Ranking function for making submission files,
        ranking events allows one to easily make ROC curves."""
    
    a = 1-a
    temp = a.argsort()
    ranks = np.empty(len(a), int)
    ranks[temp] = np.arange(len(a))
    return ranks+1

def classify_as_sb(a):
    if a == 1:
        return 's'
    return 'b'

##################################################################
##################################################################


if __name__ == '__main__':
    
    #################### Model Parameters ########################
    dump_pretrain=True
    load_pretrain=False
    dump_finetune=True
    pre_name = "pre_5x500.p"
    fine_name = "fine_5x500.p"

    finetune_lr=0.005      # learning rate for the finetuning
    training_epochs=1000   # maximum number of full scans during training
    pretraining_epochs=100 # maximun scans for pretraining each layer
    pretrain_lr=0.005       # learning rate for pretraining
    lr_reduction=0.1       # learing rate reduction factor
    tolerance=0.0001       # improvement tolerance
    batch_size=1           # size of each batch during training
    hidden_layers_sizes=[500, 500, 500, 500, 500]
    corruption_levels = [.3, .35, .4, .4, .4]

    split_level = 0.2      # % of data to be used for testing
    signal_threshold = 0.8 # minimum probability for output to be considered 'signal'
    sub_filename = 'higgs_model_sub.csv'
    
    #################### Loading Data ########################
     
    # Load data with csv reader
    training_dataset = "../training.csv" # path to training data set.
    testing_dataset  = "../test.csv"
    print '... loading training data'
    
    # Data is given as comma separated values (csv) in a standard .txt file
    data_train = np.array( list(csv.reader(open(training_dataset,'rb'), delimiter = ',')) )
    data_eval  = np.array( list(csv.reader(open(testing_dataset,'rb'), delimiter = ',')) )
    
    # Save the train and test features as numpy arrays
    X_train = np.array(data_train[1:,1:-2], float)
    X_eval  = np.array(data_eval[1:,1:], float)

    # The pretrain set contains all the unlabelled data available
    X = np.array(np.vstack((np.array(X_train), np.array(X_eval))))

    # Tease out the integer column
    Xvint = np.array(X[:,22:23], int)
    X = np.delete(X, 22, 1)

    # Set unknown float values to zero and scale the floats
    v = X  == -999.0
    X[v] = 0.
    X = preprocessing.scale(X)#, with_mean=False) 

    # Turn the four levels of ints into a binary matrix
    X = np.column_stack((X, Xvint==0, Xvint==1, Xvint==2, Xvint==3))

    # Finally add in a clustering classification
    kmean = MiniBatchKMeans(n_clusters = 6, n_init = 100, random_state=42)
    kmean.fit(X)
    Xk = kmean.predict(X)
    X = np.column_stack((X, Xk==0, Xk==1, Xk==2, Xk==3, Xk==4, Xk==5))

    # Build out the engineered training and evaluation set
    X_train = X[:len(X_train),]
    X_eval  = X[len(X_train):,]

    # Make a target set for training and a vector to hold our weights
    y_train = np.array([np.int32(row[-1] == 's') for row in data_train[1:]]) 
    w_train = np.array([float(row[-2]) for row in data_train[1:]])

    # Event ID is needed to make a submission 
    event_id = np.array(data_eval[1:,0], int)

    # Build a validation set and scale our weight vectors appropriately
    X_train, X_test, y_train, y_test, w_train, w_test = train_test_split(X_train, y_train, w_train, test_size=split_level, random_state=42)
    w_test *= 1./split_level
    w_train *= 1./(1. - split_level)
   

    ###################### Build Model ########################
    
    # compute number of minibatches for training, validation and testing
    n_train_batches = X.shape[0]
    n_train_batches /= batch_size

    # np random generator
    np_rng = np.random.RandomState(89677)
    print '... building the model'
    # construct the stacked denoising autoencoder class
    sda = SdA(numpy_rng=np_rng, n_ins=39,
              hidden_layers_sizes = hidden_layers_sizes,
              n_outs=2)

    if load_pretrain:  
        f = open(pre_name, "rb")
        sda.load(f)
        f.close()
    else:
        #########################
        # PRETRAINING THE MODEL #
        #########################

        print '... getting the pretraining functions'
        pretraining_fns = sda.pretraining_functions(train_set_x=theano.shared(X),
                                                batch_size=batch_size)

        print '... pre-training the model'
        start_time = time.clock()
        ## Pre-train layer-wise
        for i in xrange(sda.n_layers):
            last_cost = 100.
            # go through pretraining epochs
            for epoch in xrange(pretraining_epochs):
                # go through the training set
                current_lr = pretrain_lr / (1. + epoch)
                c = []
                for batch_index in xrange(n_train_batches):
                    c.append(pretraining_fns[i](index=batch_index,
                             corruption=corruption_levels[i],
                             lr=current_lr))
                current_cost = np.mean(c)
                print 'Pre-training layer %i, epoch %d, cost ' % (i, epoch),
                print current_cost
                if abs(current_cost-last_cost)<tolerance: break
                last_cost = current_cost
                

        end_time = time.clock()

        print >> sys.stderr, ('The pretraining code for file ' +
                              os.path.split(__file__)[1] +
                              ' ran for %.2fm' % ((end_time - start_time) / 60.))

        if dump_pretrain: 
            f = file(pre_name, 'wb')
            sda.save(f)
            f.close()

    ########################
    # FINETUNING THE MODEL #
    ########################

    # get the training, validation and testing function for the model
    print '... getting the finetuning functions'
    train_fn, validate_model = sda.build_finetune_functions(
                datasets=[[theano.shared(X_train), theano.shared(y_train)], [theano.shared(X_test), theano.shared(y_test)]], batch_size=batch_size)#, learning_rate=finetune_lr)

    print '... finetunning the model'
    # early-stopping parameters
    patience = 10 * n_train_batches  # look as this many examples regardless
    patience_increase = 2.  # wait this much longer when a new best is
                            # found
    improvement_threshold = 0.995  # a relative improvement of this much is
                                   # considered significant
    validation_frequency = min(n_train_batches, patience / 2)
                                  # go through this many
                                  # minibatche before checking the network
                                  # on the validation set; in this case we
                                  # check every epoch

    best_params = None
    best_validation_loss = np.inf
    start_time = time.clock()

    done_looping = False
    epoch = 0
    current_lr = finetune_lr
    last_cost = 100.

    while (epoch < training_epochs) and (not done_looping):
        epoch = epoch + 1
        current_lr = finetune_lr / (1. + epoch/3.)
        for minibatch_index in xrange(n_train_batches):
            minibatch_avg_cost = train_fn(minibatch_index, lr=current_lr)
            iter = (epoch - 1) * n_train_batches + minibatch_index

            if (iter + 1) % validation_frequency == 0:
                validation_losses = validate_model()
                this_validation_loss = np.mean(validation_losses)
                print('epoch %i, minibatch %i/%i, validation error %f %%' %
                      (epoch, minibatch_index + 1, n_train_batches,
                       this_validation_loss * 100.))
                if abs(this_validation_loss-last_cost)*100.<tolerance: 
                    done_looping = True
                last_cost = this_validation_loss


                # if we got the best validation score until now
                if this_validation_loss < best_validation_loss:
               
                    patience = 10 * n_train_batches + iter

                    #improve patience if loss improvement is good enough
                    #if (this_validation_loss < best_validation_loss *
                    #    improvement_threshold):
                    #    patience = max(patience, iter * patience_increase)

                    # save best validation score and iteration number
                    best_validation_loss = this_validation_loss
                    best_iter = iter
               
            if patience <= iter:
                done_looping = True
                break
            
    end_time = time.clock()
    print(('Optimization complete with best validation score of %f %%,') %
                 (best_validation_loss * 100.))
    print >> sys.stderr, ('The training code for file ' +
                          os.path.split(__file__)[1] +
                          ' ran for %.2fm' % ((end_time - start_time) / 60.))

    if dump_finetune: sda.save(open(fine_name, "wb"))
    ####################### Evaluating AMS metric ########################
    
    print '... checking the AMS'    
    test_predicter = sda.predicter(theano.shared(X_test))
    test_prob = np.array(test_predicter())

    y_pred = test_prob[0] > signal_threshold
    ams = AMS(y_test,y_pred,w_test)
    '''
    ams = 0
    for thresh in range(0, 20):
        thresh *= 0.05
        y_pred = test_prob[0] > thresh # Returns vector of bools: 
                                       # True if above threshold, False otherwise
        new_ams = AMS(y_test,y_pred,w_test)
        if new_ams > ams: ams, signal_threshold = new_ams, thresh
    '''
    print 'Best AMS: ', ams
    print 'Best Threshold: ', signal_threshold
    
    ####################### Make submission file ########################

      
    print '... making submission file'
    eval_predicter = sda.predicter(theano.shared(X_eval))
    pred_eval = np.array(eval_predicter())
    pred_eval = pred_eval[0]
    y_pred_eval =  pred_eval > signal_threshold
    print 'Number of submissions: ', len(pred_eval)
    pred = [classify_as_sb(y) for y in y_pred_eval] #1-->'s' and 0-->'b' for submission file
    ranks = rank(pred_eval) # submission file needs to be ranked.
    print 'Creating submission file: ', sub_filename
    f = open(sub_filename,"w")
    print >> f, "EventId,RankOrder,Class"
    for j in range(len(pred)): # print each line to submission file
        print >> f, str(event_id[j])+","+str(ranks[j])+","+str(pred[j])
    print '... done, bye'

