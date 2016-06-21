# Import the random forest package
from sklearn.ensemble import AdaBoostClassifier

import matplotlib.pyplot as plt
import csv
import numpy
from CostAMS import Cost, AMS

def Fill_Data(csvtrain):
  train = [[],[],[]]
  cv    = [[],[],[]]
  for row in csvtrain:
    rowlist = []
    for entry in row:
      rowlist.append(float(entry))
    if rowlist[0] % 5 == 0:
      cv[0].append(rowlist[1:-2])
      cv[1].append(int(rowlist[-1]))
      cv[2].append(rowlist[-2])
    else:
      train[0].append(rowlist[1:-2])
      train[1].append(int(rowlist[-1]))
      train[2].append(rowlist[-2])
  return train, cv 

def Make_Prediction(forest, csvtest):
  eventid = []
  test = []
  for row in csvtest:
    eventid.append(int(row[0]))
    test.append(row[1:])
  prediction = forest.predict(test)
  array = numpy.array(prediction)
  order = array.argsort()
  rank = order.argsort()
  return eventid, rank, prediction

def Write_Rows(eventid, rank, prediction, submission, thresh):
  submission.writerow(['EventId','RankOrder','Class'])
  for i in range(0,len(prediction)):
    if prediction[i] > thresh:
      pred='s'
    else:
      pred='b'
    submission.writerow([eventid[i],len(rank) - rank[i],pred])

def Best_Threshold(prediction, cv):
    amsbest, threshbest = 0,0
    for thresh in range(2, 18):
      thresh = thresh * 0.05
      ams = AMS(prediction, cv, thresh)
      if ams>amsbest:
        amsbest = ams
        threshbest = thresh
    return threshbest

print '... planting seeds'
csvtrain = csv.reader(open('../Data/train.csv','rb'))
csvtest  = csv.reader(open('../Data/test.csv','rb'))
csvsubmission = csv.writer(open('submission.csv','wb'))

train, cv = Fill_Data(csvtrain)
'''
cv_score, cv_ams = [], []

for n_est in range(25, 250, 25):
  print '... grooming the trees'
  # AdaBoostClassifier(base_estimator=DecisionTreeClassifier(compute_importances=None, criterion='gini', max_depth=1, max_features=None, min_density=None,min_samples_leaf=1, min_samples_split=2, random_state=None, splitter='best'), n_estimators=50, learning_rate=1.0, algorithm='SAMME.R', random_state=None)
  adaboost = AdaBoostClassifier(n_estimators = n_est, learning_rate = 1, random_state = 5).fit(train[0], train[1])
  print '... checking the fruits'
  cv_score.append(adaboost.score(cv[0], cv[1]))
  cv_ams.append(AMS(adaboost.predict(cv[0]), cv, 0.5))

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.scatter(range(25, 250, 25), cv_score, s=10, c='b', marker="s", label='score')
ax1.scatter(range(25, 250, 25), cv_ams   , s=10, c='r', marker="o", label='ams')
plt.legend(loc='upper left');
plt.show()
'''
print '... grooming the trees'
adaboost = AdaBoostClassifier(n_estimators = 200, learning_rate = 1, random_state = 5).fit(train[0], train[1])
print '... checking the fruits'
cv_prediction = adaboost.predict(cv[0])
threshbest = Best_Threshold(cv_prediction, cv)
print '... ', AMS(cv_prediction, cv, threshbest)
print '... harvesting'
eventid, rank, prediction = Make_Prediction(adaboost, csvtest)
print '... making fruit baskets'
Write_Rows(eventid, rank, prediction, csvsubmission, threshbest)

