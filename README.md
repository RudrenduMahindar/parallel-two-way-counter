# parallel-two-way-counter
Reading lines as an input from a text file (thread 1), adding each line alternatively to two queues (thread 1), getting count of vowel-consonant from each line in two queues (thread 2, thread 3), storing results of count from two queues in the output queue (thread 2, thread 3), and adding results of vowel-consonant count from the output queue (thread 4).
