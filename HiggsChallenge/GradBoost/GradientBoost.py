# Import the random forest package
from sklearn.ensemble import GradientBoostingClassifier

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

for depth in range(1, 20, 2):
  print '... grooming the trees'
  #GradientBoostingClassifier(loss='deviance', learning_rate=0.1, n_estimators=100, subsample=1.0, min_samples_split=2, min_samples_leaf=1, max_depth=3, init=None, random_state=None, max_features=None, verbose=0)
  gradboost = GradientBoostingClassifier(n_estimators = 75, learning_rate = 0.1, max_depth = depth, max_features = None).fit(train[0], train[1])
  print '... checking the fruits'
  cv_score.append(gradboost.score(cv[0], cv[1]))
  cv_ams.append(AMS(gradboost.predict(cv[0]), cv, 0.5))

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.scatter(range(1, 20, 2), cv_score, s=10, c='b', marker="s", label='score')
ax1.scatter(range(1, 20, 2), cv_ams   , s=10, c='r', marker="o", label='ams')
plt.legend(loc='upper left');
plt.show()
'''
print '... grooming the trees'
gradboost = GradientBoostingClassifier(n_estimators = 200, learning_rate = 0.5, max_depth = 7).fit(train[0], train[1])
print '... checking the fruits'
cv_prediction = gradboost.predict(cv[0])
threshbest = Best_Threshold(cv_prediction, cv)
print '... ', AMS(cv_prediction, cv, threshbest)
print '... harvesting'
eventid, rank, prediction = Make_Prediction(gradboost, csvtest)
print '... making fruit baskets'
Write_Rows(eventid, rank, prediction, csvsubmission, threshbest)


