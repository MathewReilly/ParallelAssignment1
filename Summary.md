#### Approach
To approach this problem, initially I tried factorizing each prime and dividing
the work between threads in premade sections. This approach was running too slow
and inflexible with sections being decided.

With this initial idea eliminated, I decided to use the sieve of Eratosthenes to
locate my primes by marking all of my composite values. To divide the work, I
was able to pass of an initial value for a thread to use and mark composite from,
by creating a counter that is locked over its get_and_increment method to
maintain mutual exclusion of this shared resource.

#### Correctness
##### Correctness of Output:
To check for correctness, I referred to easily checked values of primes under
100 and under 1000. This additionally, allowed me to verify the order and value
of my top ten primes. When I saw that both the number and sum of primes was correct
for these numbers, I tested the full 100 million, where I was able to verify the
number of primes under 100 million through a google search, where
https://t5k.org/howmany.html showcases the number of primes under powers of 10.

##### Safety of Critical Sections:
Mutual Exclusion is maintained through the use of mutex locks. There are two locks
protecting two separate resources, the counter's value, and the vector of composites.
While the threads access these values, they are always protected with a mutex,
preventing access to the shared resource maintaining mutual exclusion.

### Efficiency
In order for the threads to split the work, I decided to make use of the counter
idea from our first lecture. As each thread takes a number to check, some will
take numbers that have a lot of composites to mark (like 2's composites). Others
will be shorter, but also be more likely to run into the next long section to check.

Also, because every thread also needed to access the vector that marks all composite
values, I needed to also find a way to protect this section. I chose to batch
sections of marked composites together in threads so that the lock is not being
constantly acquired and released for each individual access to the vector. This
batching was far more efficient than locking an entire section or locking each
individual access.
