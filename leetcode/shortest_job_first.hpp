#ifndef SHORTEST_JOB_FIRST_HPP_INCLUDED
#define SHORTEST_JOB_FIRST_HPP_INCLUDED

#include <cstdint>
#include <vector>
#include <memory>
#include <algorithm>
#include <iostream>

class JobScheduler
{
public:
    using CpuTime = std::size_t;
    using JobId = std::size_t;

public:
    JobScheduler() = delete;

    explicit JobScheduler(std::size_t cpuNum)
    : m_cpuNum{cpuNum}, m_nextJobId{0} {
        // empty
    }

    ~JobScheduler() {
        for (auto job : m_jobs) {
            delete job;
        }
    };

    bool addJob(CpuTime spendTime) {
        Job *newJob = new Job{m_nextJobId++, 0, spendTime};
        m_jobs.insert(
            /* ÉýÐò²åÈë */
            std::lower_bound(m_jobs.begin(), m_jobs.end(), newJob, [](const Job *a, const Job *b) {
                return (a->startTime + a->spendTime) < (b->startTime + b->spendTime);
            }),
            newJob
        );
        return true;
    }

    CpuTime runAllJobs() {
        const std::size_t windownSize = std::min(m_cpuNum, m_jobs.size());
        const std::size_t moveCount = m_jobs.size() - windownSize;
        for (std::size_t i = 0; i < moveCount; ++i) {
            const Job *headJob = m_jobs[i];
            Job *tailJob = m_jobs[i + windownSize];
            tailJob->startTime = headJob->startTime + headJob->spendTime;
        }

        const Job *finalJob = m_jobs[m_jobs.size() - 1];
        return finalJob->startTime + finalJob->spendTime;
    }

    void printAllJobSchedInfo() {
        for (auto job : m_jobs) {
            std::cout << "job " << job->jobId
                      << " start time " << job->startTime
                      << " spend time " << job->spendTime
                      << " finished at " << job->startTime + job->spendTime
                      << std::endl;
        }
    }

private:
    struct Job {
        JobId   jobId;
        CpuTime startTime;
        CpuTime spendTime;
    };

    std::vector<Job*> m_jobs;
    std::size_t m_cpuNum;
    std::size_t m_nextJobId;
};

#endif // SHORTEST_JOB_FIRST_HPP_INCLUDED
