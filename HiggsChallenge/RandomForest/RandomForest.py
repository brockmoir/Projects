# Import the random forest package
from sklearn.ensemble import RandomForestClassifier 

import matplotlib.pyplot as plt
import csv
import numpy
import itertools

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

print '... driving to the greenhouse'
csvtrain = csv.reader(open('../Data/train.csv','rb'))
csvtest  = csv.reader(open('../Data/test.csv','rb'))
csvsubmission = csv.writer(open('submission.csv','wb'))

train, cv = Fill_Data(csvtrain)

print '... planting seeds and growing forests'

forest = RandomForestClassifier(n_estimators = 100, max_features = 30, max_depth = None, min_samples_split = 1, n_jobs = -1)
forest = forest.fit(train[0], train[1])

cv_prediction = forest.predict(cv[0])
threshbest = Best_Threshold(cv_prediction, cv)

eventid, rank, prediction = Make_Prediction(forest, csvtest)
Write_Rows(eventid, rank, prediction, csvsubmission, threshbest)


'''
train_cost, train_ams = [], []
cv_cost, cv_ams = [], []

for i in range(0, 100000, 5000):
  part_train = [[],[],[]]
  forest = RandomForestClassifier(n_estimators = 100, max_features = 30, max_depth = None, min_samples_split = 1, n_jobs = -1)
  for j in range(i+5000):
    part_train[0].append(train[0][j])
    part_train[1].append(train[1][j])
    part_train[2].append(train[2][j])   
  forest = forest.fit(part_train[0], part_train[1])
  train_prediction = forest.predict(part_train[0])
  train_cost.append(Cost(train_prediction, part_train[1]))
  train_ams.append(AMS(train_prediction, part_train, 0.5))
  cv_prediction    = forest.predict(cv[0])
  cv_cost.append(Cost(cv_prediction, cv[1]))
  cv_ams.append(AMS(cv_prediction, cv, 0.5))

fig = plt.figure()
ax1 = fig.add_subplot(111)
ax1.scatter(range(0, 100000, 5000), cv_cost, s=10, c='b', marker="s", label='train')
ax1.scatter(range(0, 100000, 5000), cv_ams   , s=10, c='r', marker="o", label='cv')
plt.legend(loc='upper left');
plt.show()
'''


