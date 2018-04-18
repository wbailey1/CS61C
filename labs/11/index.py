import sys
import re

from pyspark import SparkContext

def flat_map(document):
    """
    Takes in document, which is a key, value pair, where document[0] is the
    document ID and document[1] is the contents of the document.
    HINT: You need to keep track of three things, word, document ID, and the
    index inside of the document, but you are working with key, value pairs.
    Is there a way to combine these three things and make a key, value pair?
    """
    """ Your code here. """
    docTokens = re.findall(r"\w+", document[1])
    result = list()
    for index,element in enumerate(docTokens):
        final = (element,(document[0],index))
        result.append(final)

    return result

def map(arg):
    """ Your code here. """
    key = (arg[0],arg[1][0])
    value = [arg[1][1]]
    return (key,value)

def reduce(arg1, arg2):
    """ Your code here. """
    listMerge = arg1 + arg2 

    return listMerge

def index(file_name, output="spark-wc-out-index"):
    sc = SparkContext("local[8]", "Index")
    file = sc.sequenceFile(file_name)

    indices = file.flatMap(flat_map) \
                  .map(map) \
                  .reduceByKey(reduce).sortByKey()

    indices.coalesce(1).saveAsTextFile(output)

""" Do not worry about this """
if __name__ == "__main__":
    argv = sys.argv
    if len(argv) == 2:
        index(argv[1])
    else:
        index(argv[1], argv[2])