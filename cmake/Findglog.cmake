unset(glog_FOUND)
unset(glog_INCLUDE_DIRS)
unset(glog_LIBRARIES)

find_path(glog_INCLUDE_DIRS NAMES
    logging.h
    PATHS
    /usr/include/glog
)

find_library(glog_LIBRARIES NAMES
    glog
    PATHS
    /usr/lib/x86_64-linux-gnu
)

if (glog_INCLUDE_DIRS AND glog_LIBRARIES)
    message(STATUS "glog found in system")
    set(glog_FOUND 1)
endif (glog_INCLUDE_DIRS AND glog_LIBRARIES)
