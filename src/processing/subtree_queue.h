/**
 * @file        subtree_queue.h
 *
 * @author      Tobias Anker <tobias.anker@kitsunemimi.moe>
 *
 * @copyright   Apache License Version 2.0
 *
 *      Copyright 2019 Tobias Anker
 *
 *      Licensed under the Apache License, Version 2.0 (the "License");
 *      you may not use this file except in compliance with the License.
 *      You may obtain a copy of the License at
 *
 *          http://www.apache.org/licenses/LICENSE-2.0
 *
 *      Unless required by applicable law or agreed to in writing, software
 *      distributed under the License is distributed on an "AS IS" BASIS,
 *      WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *      See the License for the specific language governing permissions and
 *      limitations under the License.
 */

#ifndef KITSUNEMIMI_SAKURA_LANG_SUBTREE_QUEUE_H
#define KITSUNEMIMI_SAKURA_LANG_SUBTREE_QUEUE_H

#include <thread>
#include <chrono>
#include <mutex>
#include <queue>

#include <items/sakura_items.h>

namespace Kitsunemimi
{
namespace Sakura
{
class SakuraItem;

typedef std::chrono::microseconds chronoMicroSec;
typedef std::chrono::milliseconds chronoMilliSec;
typedef std::chrono::nanoseconds chronoNanoSec;
typedef std::chrono::seconds chronoSec;
typedef std::chrono::high_resolution_clock::time_point chronoTimePoint;
typedef std::chrono::high_resolution_clock chronoClock;

class SubtreeQueue
{
public:
    SubtreeQueue();

    /**
     * @brief The ActiveCounter struct is only a simple thread-save counter. This counter should be
     *        increased, after the subtree was fully processed. All subtree-queue objects,
     *        which have the same source and belong to each other, share the same instance
     *        of this counter. With this, the source-thread should be able to check, that all its
     *        spawn subtree-objects have finished their task before increasing this counter.
     */
    struct ActiveCounter
    {
        std::mutex lock;
        uint32_t isCounter = 0;
        uint32_t shouldCount = 0;
        bool success = true;
        std::string outputMessage = "";

        ActiveCounter() {}

        /**
         * @brief increase the counter
         */
        void increaseCounter()
        {
            lock.lock();
            isCounter++;
            lock.unlock();
        }

        /**
         * @brief check, that the counter has reached the expected value
         *
         * @return true, if counter has reached the expected value, else false
         */
        bool isEqual()
        {
            bool result = false;
            lock.lock();
            result = isCounter == shouldCount;
            lock.unlock();
            return result;
        }

        /**
         * @brief register error in one of the spawned threads to inform the other threads
         *
         * @param errorMessage error-message
         */
        void registerError(const std::string &errorMessage)
        {
            lock.lock();
            success = false;
            outputMessage = errorMessage;
            lock.unlock();
        }
    };

    /**
     * @brief The SubtreeObject struct is basically a container to encapsulate a task. It contains
     *        all necessary information to process a subtree. These container are placed in the
     *        queue, from where they are taken by the worker-threads.
     */
    struct SubtreeObject
    {
        // subtree, which should be processed by a worker-thread
        SakuraItem* subtree = nullptr;
        // map with all input-values for the subtree
        DataMap items;
        // shared counter-instance, which will be increased after the subtree was fully processed
        ActiveCounter* activeCounter = nullptr;
        // current position in the processing-hirarchy for status-output
        std::vector<std::string> hirarchy;

        std::string filePath = "";
    };

    void addSubtreeObject(SubtreeObject* newObject);

    bool spawnParallelSubtrees(DataMap &resultingItems,
                               const std::vector<SakuraItem *> &childs,
                               const std::string &filePath,
                               const std::vector<std::string> &hierarchy,
                               const DataMap &parentValues,
                               std::string &errorMessage,
                               const uint64_t endPos = 1,
                               const uint64_t startPos = 0);
    bool spawnParallelSubtreesLoop(SakuraItem* subtree,
                                   ValueItemMap postProcessing,
                                   const std::string &filePath,
                                   const std::vector<std::string> &hierarchy,
                                   DataMap &parentValues,
                                   const std::string &tempVarName,
                                   DataArray* array,
                                   std::string &errorMessage,
                                   uint64_t endPos,
                                   const uint64_t startPos = 0);



    SubtreeObject* getSubtreeObject();

private:
    std::mutex m_lock;
    std::queue<SubtreeObject*> m_queue;

    bool waitUntilFinish(ActiveCounter* activeCounter,
                         std::string &errorMessage);
    void clearSpawnedObjects(std::vector<SubtreeObject*> &spawnedObjects);
};

} // namespace Sakura
} // namespace Kitsunemimi

#endif // KITSUNEMIMI_SAKURA_LANG_SUBTREE_QUEUE_H
