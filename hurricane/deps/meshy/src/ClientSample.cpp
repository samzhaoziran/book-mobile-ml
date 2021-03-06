/**
 * licensed to the apache software foundation (asf) under one
 * or more contributor license agreements.  see the notice file
 * distributed with this work for additional information
 * regarding copyright ownership.  the asf licenses this file
 * to you under the apache license, version 2.0 (the
 * "license"); you may not use this file except in compliance
 * with the license.  you may obtain a copy of the license at
 *
 * http://www.apache.org/licenses/license-2.0
 *
 * unless required by applicable law or agreed to in writing, software
 * distributed under the license is distributed on an "as is" basis,
 * without warranties or conditions of any kind, either express or implied.
 * see the license for the specific language governing permissions and
 * limitations under the license.
 */

#include "Net.h"
#include "EventQueue.h"
#include "EventQueueLoop.h"
#include "IoLoop.h"
#include "PackageDataSink.h"
#include "epoll/EPollServer.h"
#include "epoll/EPollClient.h"
#include "logging/Logging.h"

#include <vector>
#include <iostream>
#include <string>
#include <thread>
#include <memory>

using namespace std;

const int32_t DefaultPort = 9000;
class SampleEventQueueLoop : public meshy::EventQueueLoop {
public:
    SampleEventQueueLoop(meshy::EventQueue* eventQueue) :
            EventQueueLoop(eventQueue) {}

protected:
    virtual void OnEvent(std::shared_ptr<meshy::BaseEvent> event) override {
        /*
       LOG(KLOG_DEBUG) << ("SampleEventQueueLoop::OnEvent");
        char buf[BUFSIZ]; // BUFSIZ is 1024
        sprintf(buf, "HTTP/1.1 200 OK\r\nContent-Length: %d\r\n\r\nHello World", 11);

        event->GetConnection()->Send(ByteArray(buf, strlen(buf)));
        LOG(KLOG_DEBUG) << ("SampleEventQueueLoop::OnEvent end");
         */
    }
};

using hurricane::logging::Logging;

int32_t main() {
    Logging::SetLogFile("client-all.log");
    Logging::SetLogFile({ LOG_INFO, LOG_DEBUG }, "client-output.log");
    Logging::SetLogFile({ LOG_WARNING, LOG_ERROR, LOG_FATAL }, "client-error.log");

    meshy::EventQueue mainEventQueue(5);
    meshy::IoLoop::Get()->Start();

    meshy::PackageDataSink dataSink(&mainEventQueue);

    meshy::EPollClientPtr client = meshy::EPollClient::Connect("127.0.0.1", DefaultPort, &dataSink);

    client->Send(meshy::ByteArray("hello", 5));

    SampleEventQueueLoop sampleQueue(&mainEventQueue);
    sampleQueue.Start();

    return 0;
}
