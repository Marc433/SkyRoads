#ifndef __JOB_SYSTEM__
#define __JOB_SYSTEM__ 1

#include <vector>
#include <list>
#include <unordered_map>
#include <functional>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>

/**
 * @class JobSystem
 * @brief A multi-threaded job system for task parallelization.
 *
 * The JobSystem allows for the queuing and execution of different types of tasks
 * with varying priorities across multiple worker threads. It's designed to efficiently
 * distribute workloads in areas like rendering, physics, AI, and audio processing.
 */
class JobSystem {
public:
  /**
   * @enum JobType
   * @brief Categorizes jobs into different subsystems.
   */
  enum JobType {
    Render,    ///< Jobs related to rendering
    Physics,   ///< Jobs related to physics calculations
    AI,        ///< Jobs related to artificial intelligence
    Audio,     ///< Jobs related to audio processing
    General    ///< General-purpose jobs
  };

  /**
   * @enum JobPriority
   * @brief Defines priority levels for jobs.
   */
  enum JobPriority {
    Low,      ///< Low priority jobs, executed after medium and high priority jobs
    Medium,   ///< Medium priority jobs
    High      ///< High priority jobs, executed before medium and low priority jobs
  };

  /**
   * @struct PrioritizedJob
   * @brief A job with an associated priority level.
   */
  struct PrioritizedJob {
    JobPriority priority;          ///< The priority level of the job
    std::function<void()> job;     ///< The job function to execute

    /**
     * @brief Comparison operator for prioritizing jobs.
     *
     * @param other The job to compare against.
     * @return true if this job has lower priority than the other job.
     */
    bool operator<(const PrioritizedJob& other) const {
      return static_cast<int>(priority) < static_cast<int>(other.priority);
    }
  };

  /**
   * @brief Constructor that initializes the job system with a specific number of worker threads.
   *
   * @param numThreads The number of worker threads to create.
   */
  JobSystem(unsigned int numThreads);

  /**
   * @brief Destructor that cleans up worker threads and pending jobs.
   */
  ~JobSystem();

  /**
   * @brief Copy constructor.
   *
   * @param other The JobSystem to copy from.
   */
  JobSystem(const JobSystem& other);

  /**
   * @brief Move constructor.
   *
   * @param other The JobSystem to move from.
   */
  JobSystem(JobSystem& other);

  /**
   * @brief Adds a new job to the job system.
   *
   * @param type The type of job, which determines which queue it's added to.
   * @param priority The priority level of the job.
   * @param f The function to execute as the job.
   */
  void addJob(JobType type, JobPriority priority, std::function<void()> f);

  /**
   * @brief Executes pending tasks.
   *
   * This method is called by worker threads to process jobs.
   */
  void runTasks();

  /**
   * @brief The worker threads that process jobs.
   */
  std::vector<std::thread> workers;

private:
  /**
   * @brief Maps job types to their respective queues of prioritized jobs.
   */
  std::unordered_map<JobType, std::list<PrioritizedJob>> tasks;

  /**
   * @brief Mutex for thread synchronization.
   *
   * Used to protect access to the task queues.
   */
  std::mutex mutex;

  /**
   * @brief Flag indicating whether the job system should shut down.
   */
  std::atomic<bool> quit;

  /**
   * @brief Condition variable for worker threads.
   *
   * Used to wake up worker threads when new jobs arrive.
   */
  std::condition_variable condition;
};

#endif // !#define __JOB_SYSTEM__ 1