//
// Created by whs31 on 06.09.23.
//

#pragma once

#define invokable Q_INVOKABLE
#define in :
#define MODEL_READ_ERROR "Error reading from model"
#define CONCURRENT_RUN QFuture<void> future = QtConcurrent::run(
#define CONCURRENT_ARGS (){ QFuture<void> future_priv = QtConcurrent::run(
#define CONCURRENT_RUN_START (){
#define CONCURRENT_RUN_END });});
#define CONCURRENT_RUN_END_WATCHER(watcher) CONCURRENT_RUN_END \
                                            watcher.setFuture(future);