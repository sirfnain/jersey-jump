/*
 * Copyright 2013 BlackLocus
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 */

package com.blacklocus.webapp.base;

import javax.ws.rs.ext.Provider;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.LinkedBlockingQueue;
import java.util.concurrent.SynchronousQueue;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.concurrent.TimeUnit;

/**
 * General purpose thread pools
 *
 * @author Jason Dunkelberger (dirkraft)
 */
@Provider
public class ExecutorServiceProvider {

    private final ExecutorService roots = new ThreadPoolExecutor(8, 8, 1, TimeUnit.HOURS,
            new SynchronousQueue<Runnable>());

    private final ExecutorService monitors = Executors.newCachedThreadPool();

    private final ExecutorService workers = new ThreadPoolExecutor(32, 32, 1, TimeUnit.HOURS,
            new LinkedBlockingQueue<Runnable>(256));

    /**
     * @return executor service for root level tasks or monitors. The number of concurrent processes is severely limited.
     */
    public ExecutorService forRoots() {
        return roots;
    }

    /**
     * @return an unbounded executor service for lightweight, monitoring-purposed processes. Root-level monitors should
     *         still be given to {@link #forRoots()} (e.g. the top of a Job), but component monitors that themselves
     *         don't do any heavy lifting may be given to this pool.
     */
    public ExecutorService forMonitors() {
        return monitors;
    }

    /**
     * @return executor service that limits number of concurrent process. Should be used with worker-level tasks to
     *         rate control the amount of resource contention.
     */
    public ExecutorService forWorkers() {
        return workers;
    }
}
