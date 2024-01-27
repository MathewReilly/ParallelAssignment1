#### Approach
I decided to use the sieve of Eratosthenes to locate my primes by marking all of
my composite values. Each thread would be responsible for marking composites in an
boolean vector of size "n" with each value initialized to 0.

#### Correctness
##### Correctness of Output:
To check for correctness, I referred to easily checked values of primes under
100 and under 1000. This also, allowed me to verify the order and value of my top
ten primes. Additionally, when I tested the full 100 million, I was able to verify
the count of primes under 100 million through a google search, where
https://t5k.org/howmany.html showcases the number of primes under powers of 10.

##### Safety of Critical Sections:
Mutual Exclusion is maintained through the use of mutex locks. There are two locks
protecting two separate resources, the counter's value, and the vector of composites.
While the threads access these values, they are always protected with a mutex,
maintaining mutual exclusion.

### Efficiency
I decided to make use of the counter idea from our first lecture to divide work.
As each thread takes a number to check, some will take numbers that have a lot of
composites to mark (like 2's composites). Others will be shorter, but also be more
likely to run into the next long section to check, balancing out work.

Because every thread needs to access the vector that marks all composite values,
I had to protect this section. I chose to batch sections of collected composites
and update marked composite from each batch. Locking these batches was far more
efficient than locking an entire section or locking each individual access.
