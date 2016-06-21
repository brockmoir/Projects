import math

def CostStep(prediction, truth):
  if abs(prediction - truth) > 0.999999999999:
    return 10. 
  elif abs(prediction - truth) < 0.000000000001:
    return 0.
  return -truth*math.log(prediction)-(1.-truth)*math.log(1.-prediction)

def Cost(prediction, truth):
  cost = 0
  for i in range(0,len(prediction)):
    cost = cost + CostStep(prediction[i], truth[i])
  return cost / len(prediction)

def AMS(prediction, truthset, thresh):
  s,b = 0,0
  for i in range(0,len(prediction)):
    if prediction[i] > thresh and truthset[1][i] == 1:
      s = s + 1 * truthset[2][i]
    if prediction[i] > thresh and truthset[1][i] == 0:
      b = b + 1 * truthset[2][i]
  return 4 * ((s + b + 10.) * math.log(1. + s / (b + 10)) - s) ** 0.5
