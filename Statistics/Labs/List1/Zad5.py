# import math
import numpy as np
import statistics as stat
from scipy import optimize
from tabulate import tabulate

def dlog(theta, sigma, xi):
    n = len(xi)
    ans = 0
    for x in xi:
        tmpexp = np.exp((theta - x) / sigma)
        ans += tmpexp / (1 + tmpexp)
    return (n - 2 * ans) / sigma

def ddlog(theta, sigma, xi):
    ans = 0
    for x in xi:
        tmpexp = np.exp((theta - x) / sigma)
        ans += tmpexp / ((1 + tmpexp) ** 2)
    return (-2 / (sigma ** 2)) * ans

# number_of_iterations = 0
# def count_iterations(xk):
#     global number_of_iterations
#     number_of_iterations += 1


def probe(n, loc, scale):
    dataset = np.random.logistic(loc=loc, scale=scale, size=n)
    x0 = stat.mean(dataset)
    # print(number_of_iterations)
    root, rootResults = optimize.newton(dlog, x0, fprime = ddlog, args = (scale, dataset), full_output=True)
    # print(rootResults.iterations)
    # estimators = [get_est1(dataset), get_est2(dataset), get_est3(dataset, my_weights), get_est4(dataset)]
    return root, rootResults.iterations

def experiment(n, loc, scale):
    number_of_probes = 500
    estimators = []
    # dataset = probe(n,)
    average_iterations = 0
    for i in range(number_of_probes):
        est, iterations = probe(n, loc, scale)
        estimators.append(est)
        average_iterations += iterations

    average_iterations /= number_of_probes
    print(average_iterations)
    return estimators
    # return [list(estimator) for estimator in list(zip(*estimators))]

def main():
    cases = [['A',50,1,1],['B',50,4,1],['C',50,1,2]]
    for case in cases:
        print(f"Case {case[0]}:")
        results = experiment(case[1], case[2], case[3])
        table = [['Bias', 'Var', 'MSE']]
        
        # estimators = ['Mean', 'Med', 'random weights', 'gauss weights']
        # for i in range(len(results)):
        tmptable = []
        tmptable.append(np.mean(results) - case[2])                     # BIAS
        tmptable.append(np.var(results))                                # VAR
        tmptable.append(np.mean((np.array(results) - case[2]) ** 2))    # MSE
        table.append(tmptable)
        print(tabulate(table, headers="firstrow", tablefmt="grid"))
        # print(table)

main()

# root = optimize.newton(dlog, 1, fprime2 = ddlog, args = )