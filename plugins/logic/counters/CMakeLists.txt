cmake_minimum_required(VERSION 3.9 FATAL_ERROR)

project(LogicCounters VERSION 17.09.06 LANGUAGES C CXX)

set(SOURCES
    count_up/count_up.cc
    count_up/count_up.h
    count_down/count_down.cc
    count_down/count_down.h
    count_between/count_between.cc
    count_between/count_between.h

    counters.cc
)

include(PluginBuild)
