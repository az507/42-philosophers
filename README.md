# Philosophers

> A 42 School project that explores concurrency, threads, and mutexes via the classic Dining Philosophers problem.

## Project Description

The **Philosophers** project is designed to help students understand the basic concepts of concurrency and synchronization. The goal is to prevent deadlocks and race conditions while simulating a group of philosophers sitting at a table and eating, thinking, and sleeping in a controlled and thread-safe manner.

## The Dining Philosophers Problem

Five philosophers sit at a round table with a fork between each pair. Each philosopher:
- Thinks 🤔
- Picks up two forks (one on the left and one on the right) 🍴
- Eats 🍝
- Puts down the forks
- Sleeps 🛌

The challenge is to make sure that:
- No two philosophers hold the same fork at the same time.
- There are no deadlocks (i.e., all philosophers waiting forever).
- All threads run concurrently and efficiently.

## Implementation Details

- **Language:** C
- **Threads:** `pthread`
- **Synchronization:** Mutexes (`pthread_mutex_t`)

## Usage
```bash
git clone https://github.com/az507/42-philosophers.git
cd 42-philosophers/philo
make
./philo [number_of_philosophers] [time_to_die] [time_to_eat] [time_to_sleep] [number_of_times_each_philosopher_must_eat]
./philo 5 800 200 200 # eg
```

## Command-line Arguments
| Argument | Description |
| -------- | ----------- |
| `number_of_philosophers` | 	The number of philosophers and also the number of forks |
| `time_to_die` (in ms) | 	Time a philosopher can live without eating before dying |
| `time_to_eat` (in ms) | 	Time it takes a philosopher to eat |
| `time_to_sleep` (in ms) | Time a philosopher will spend sleeping |
| `number_of_times_each_philosopher_must_eat ` (*optional*) | If specified, simulation stops when each philosopher has eaten this many times |

## Bonus

> Work in progress
