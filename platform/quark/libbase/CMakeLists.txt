cmake_minimum_required(VERSION 3.5)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(base)
enable_language(CXX)

set(LIBRARY_OUTPUT_PATH "output")
set(BASE_TARGET_NAME "base")
set(CMAKE_CXX_COMPILER g++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")

if(x86 MATCHES (${BASE_ARCH_PLATFORM}))
    if(Linux MATCHES (${BASE_BUILD_PLATFORM}) OR Android MATCHES (${BASE_BUILD_PLATFORM}))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m32")
    endif()

    if(macOSX MATCHES (${BASE_BUILD_PLATFORM}))
        set(CMAKE_OSX_ARCHITECTURES x86)
    endif()
endif()

if(x64 MATCHES (${BASE_ARCH_PLATFORM}))
    if(Linux MATCHES (${BASE_BUILD_PLATFORM}) OR Android MATCHES (${BASE_BUILD_PLATFORM}))
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -lp64")
    endif()

    if(macOSX MATCHES (${BASE_BUILD_PLATFORM}))
        set(CMAKE_OSX_ARCHITECTURES x86_64)
    endif()
endif()

message(STATUS *******************************************************)
message(STATUS CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME})
message(STATUS CMAKE_SYSTEM_VERSION=${CMAKE_SYSTEM_VERSION})
message(STATUS CMAKE_HOST_SYSTEM_NAME=${CMAKE_HOST_SYSTEM_NAME})
message(STATUS CMAKE_GENERATOR=${CMAKE_GENERATOR})
message(STATUS CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
message(STATUS CMAKE_CONFIGURATION_TYPES=${CMAKE_CONFIGURATION_TYPES})
message(STATUS CMAKE_C_COMPILER=${CMAKE_C_COMPILER})
message(STATUS CMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER})
message(STATUS CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES=${CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES})
message(STATUS CMAKE_CXX_STANDARD_LIBRARIES=${CMAKE_CXX_STANDARD_LIBRARIES})
message(STATUS CMAKE_C_FLAGS=${CMAKE_C_FLAGS})
message(STATUS CMAKE_CXX_FLAGS=${CMAKE_CXX_FLAGS})
message(STATUS CMAKE_SHARED_LINKER_FLAGS=${CMAKE_SHARED_LINKER_FLAGS})
message(STATUS CMAKE_MODULE_LINKER_FLAGS=${CMAKE_MODULE_LINKER_FLAGS})
message(STATUS CMAKE_EXE_LINKER_FLAGS=${CMAKE_EXE_LINKER_FLAGS})
message(STATUS CMAKE_BINARY_DIR=${CMAKE_BINARY_DIR})
message(STATUS CMAKE_SOURCE_DIR=${CMAKE_SOURCE_DIR})
message(STATUS CMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE})
message(STATUS CMAKE_SYSTEM_NAME=${CMAKE_SYSTEM_NAME})
message(STATUS CMAKE_FIND_LIBRARY_SUFFIXES=${CMAKE_FIND_LIBRARY_SUFFIXES})
message(STATUS CMAKE_STATIC_LIBRARY_PREFIX=${CMAKE_STATIC_LIBRARY_PREFIX})
message(STATUS *******************************************************)

set(BASE_PLATFORM_SOURCE)
set(BASE_PLATFORM_DEFINITIONS)
set(BASE_LINK_PLATFORM_LIBRARIES)
set(BASE_INCLUDE_PLATFORM_DIRECTORIES)
if(Linux MATCHES (${BASE_BUILD_PLATFORM}))
    set(BASE_BUILD_CONFIGURATION_TYPE SHARED)

    set(BASE_PLATFORM_DEFINITIONS 
        -DUSE_SYSTEM_LIBEVENT
    )

    set(BASE_LINK_PLATFORM_LIBRARIES    
        glib-2.0
        pthread
    )

    set(BASE_INCLUDE_PLATFORM_DIRECTORIES   
        /usr/local/include/glib-2.0
        /usr/local/lib/glib-2.0/include # for glibconfig.h
    )

    set(BASE_PLATFORM_SOURCE  
        base/base_paths_posix.cc
        base/base_paths_posix.h
        base/file_descriptor_posix.h
        base/file_util_posix.cc
        base/global_descriptors_posix.cc
        base/global_descriptors_posix.h
        base/guid_posix.cc
        base/hi_res_timer_manager_posix.cc
        base/native_library_posix.cc
        base/platform_file_posix.cc
        base/process_posix.cc
        base/process_util_posix.cc
        base/rand_util_posix.cc
        base/safe_strerror_posix.cc
        base/safe_strerror_posix.h
        base/shared_memory_posix.cc
        base/string_util_posix.h
        base/sync_socket_posix.cc
        base/sys_info_posix.cc
        base/sys_string_conversions_posix.cc
        base/time_posix.cc
        base/debug/stack_trace_posix.cc
        base/files/dir_reader_posix.h
        base/synchronization/condition_variable_posix.cc
        base/synchronization/lock_impl_posix.cc
        base/synchronization/waitable_event_posix.cc
        base/synchronization/waitable_event_watcher_posix.cc
        base/system_monitor/system_monitor_posix.cc
        base/threading/platform_thread_posix.cc
        base/threading/thread_local_posix.cc
        base/threading/thread_local_storage_posix.cc
        base/threading/worker_pool_posix.cc
        base/threading/worker_pool_posix.h
        base/file_util_linux.cc
        base/linux_util.cc
        base/linux_util.h
        base/process_linux.cc
        base/process_util_linux.cc
        base/sys_info_linux.cc
        base/files/dir_reader_linux.h
        base/files/file_path_watcher_linux.cc
        base/third_party/nspr/prcpucfg_linux.h
        base/message_pump_libevent.h
    )
endif()

set(BASE_INCLUDE_DIRECTORIES    
    .
    ./base
    ./build
)

set(BASE_LINK_DIRECTORIES   
)

set(BASE_DEFINITIONS    
    -DBASE_IMPLEMENTATION	
    -DFILE_OFFSET_BITS=64
    ${BASE_PLATFORM_DEFINITIONS}
)

set(BASE_SOURCE 
      base/at_exit.cc
      base/at_exit.h
      base/atomic_ref_count.h
      base/atomicops.h
      base/atomicops_internals_arm_gcc.h
      base/atomicops_internals_atomicword_compat.h
      base/atomicops_internals_gcc.h
      base/atomicops_internals_tsan.h
      base/atomicops_internals_x86_gcc.cc
      base/atomicops_internals_x86_gcc.h
      base/base_export.h
      base/base_paths.cc
      base/base_paths_posix.cc
      base/base_switches.cc
      base/base_switches.h
      base/basictypes.h
      base/bind.h
      base/bind_helpers.cc
      base/bind_helpers.h
      base/bind_internal.h
      base/callback.h
      base/callback_forward.h
      base/callback_helpers.cc
      base/callback_helpers.h
      base/callback_internal.cc
      base/callback_internal.h
      base/callback_registry.h
      base/cancelable_callback.h
      base/command_line.cc
      base/command_line.h
      base/compiler_specific.h
      base/containers/hash_tables.h
      base/containers/linked_list.h
      base/containers/mru_cache.h
      base/containers/scoped_ptr_hash_map.h
      base/containers/small_map.h
      base/containers/stack_container.h
      base/cpu.cc
      base/cpu.h
      base/debug/alias.cc
      base/debug/alias.h
      base/debug/asan_invalid_access.cc
      base/debug/asan_invalid_access.h
      base/debug/crash_logging.cc
      base/debug/crash_logging.h
      base/debug/debugger.cc
      base/debug/debugger.h
      base/debug/debugger_posix.cc
      base/debug/dump_without_crashing.cc
      base/debug/dump_without_crashing.h
      base/debug/leak_annotations.h
      base/debug/leak_tracker.h
      base/debug/proc_maps_linux.cc
      base/debug/proc_maps_linux.h
      base/debug/profiler.cc
      base/debug/profiler.h
      base/debug/stack_trace.cc
      base/debug/stack_trace.h
      base/debug/stack_trace_posix.cc
      base/debug/task_annotator.cc
      base/debug/task_annotator.h
      base/debug/trace_event.h
      base/debug/trace_event_argument.cc
      base/debug/trace_event_argument.h
      base/debug/trace_event_impl.cc
      base/debug/trace_event_impl.h
      base/debug/trace_event_impl_constants.cc
      base/debug/trace_event_memory.cc
      base/debug/trace_event_memory.h
      base/debug/trace_event_synthetic_delay.cc
      base/debug/trace_event_synthetic_delay.h
      base/debug/trace_event_system_stats_monitor.cc
      base/debug/trace_event_system_stats_monitor.h
      base/environment.cc
      base/files/file.cc
      base/files/file.h
      base/files/file_enumerator.cc
      base/files/file_enumerator.h
      base/files/file_enumerator_posix.cc
      base/files/file_path.cc
      base/files/file_path.h
      base/files/file_path_constants.cc
      base/files/file_posix.cc
      base/files/file_util.cc
      base/files/file_util.h
      base/files/file_util_linux.cc
      base/files/file_util_posix.cc
      base/files/scoped_file.cc
      base/files/scoped_file.h
      base/files/scoped_temp_dir.cc
      base/files/scoped_temp_dir.h
      base/hash.cc
      base/hash.h
      base/json/json_file_value_serializer.cc
      base/json/json_file_value_serializer.h
      base/json/json_parser.cc
      base/json/json_parser.h
      base/json/json_reader.cc
      base/json/json_reader.h
      base/json/json_string_value_serializer.cc
      base/json/json_string_value_serializer.h
      base/json/json_value_converter.cc
      base/json/json_value_converter.h
      base/json/json_writer.cc
      base/json/json_writer.h
      base/json/string_escape.cc
      base/json/string_escape.h
      base/lazy_instance.cc
      base/lazy_instance.h
      base/location.cc
      base/location.h
      base/logging.cc
      base/logging.h
      base/md5.cc
      base/memory/raw_scoped_refptr_mismatch_checker.h
      base/memory/ref_counted.cc
      base/memory/ref_counted.h
      base/memory/ref_counted_memory.cc
      base/memory/ref_counted_memory.h
      base/memory/scoped_handle.h
      base/memory/scoped_policy.h
      base/memory/scoped_ptr.h
      base/memory/scoped_vector.h
      base/memory/singleton.cc
      base/memory/singleton.h
      base/memory/weak_ptr.cc
      base/memory/weak_ptr.h
      base/message_loop/incoming_task_queue.cc
      base/message_loop/incoming_task_queue.h
      base/message_loop/message_loop.cc
      base/message_loop/message_loop.h
      base/message_loop/message_loop_proxy.cc
      base/message_loop/message_loop_proxy.h
      base/message_loop/message_loop_proxy_impl.cc
      base/message_loop/message_loop_proxy_impl.h
      base/message_loop/message_pump.cc
      base/message_loop/message_pump.h
      base/message_loop/message_pump_default.cc
      base/message_loop/message_pump_default.h
      base/message_loop/message_pump_libevent.cc
      base/message_loop/message_pump_libevent.h
      base/message_loop/timer_slack.h
      base/move.h
      base/path_service.cc
      base/path_service.h
      base/pending_task.cc
      base/pending_task.h
      base/pickle.cc
      base/pickle.h
      base/port.h
      base/process/internal_linux.cc
      base/process/kill.cc
      base/process/kill.h
      base/process/kill_posix.cc
      base/process/launch.cc
      base/process/launch.h
      base/process/launch_posix.cc
      base/process/memory.cc
      base/process/memory.h
      base/process/memory_linux.cc
      base/process/process.h
      base/process/process_handle.h
      base/process/process_handle_linux.cc
      base/process/process_handle_posix.cc
      base/process/process_iterator.cc
      base/process/process_iterator.h
      base/process/process_iterator_linux.cc
      base/process/process_linux.cc
      base/process/process_metrics.cc
      base/process/process_metrics_linux.cc
      base/process/process_metrics_posix.cc
      base/profiler/alternate_timer.cc
      base/profiler/alternate_timer.h
      base/profiler/scoped_profile.cc
      base/profiler/scoped_profile.h
      base/profiler/scoped_tracker.cc
      base/profiler/scoped_tracker.h
      base/profiler/tracked_time.cc
      base/profiler/tracked_time.h
      base/rand_util.cc
      base/rand_util.h
      base/rand_util_posix.cc
      base/run_loop.cc
      base/run_loop.h
      base/safe_strerror_posix.cc
      base/safe_strerror_posix.h
      base/scoped_clear_errno.h
      base/sequence_checker.h
      base/sequence_checker_impl.cc
      base/sequence_checker_impl.h
      base/sequenced_task_runner.cc
      base/sequenced_task_runner.h
      base/stl_util.h
      base/strings/string16.cc
      base/strings/string16.h
      base/strings/string_number_conversions.cc
      base/strings/string_number_conversions.h
      base/strings/string_piece.cc
      base/strings/string_piece.h
      base/strings/string_split.cc
      base/strings/string_split.h
      base/strings/string_tokenizer.h
      base/strings/string_util.cc
      base/strings/string_util.h
      base/strings/string_util_constants.cc
      base/strings/string_util_posix.h
      base/strings/stringize_macros.h
      base/strings/stringprintf.cc
      base/strings/stringprintf.h
      base/strings/sys_string_conversions.h
      base/strings/sys_string_conversions_posix.cc
      base/strings/utf_string_conversion_utils.cc
      base/strings/utf_string_conversion_utils.h
      base/strings/utf_string_conversions.cc
      base/strings/utf_string_conversions.h
      base/synchronization/cancellation_flag.cc
      base/synchronization/cancellation_flag.h
      base/synchronization/condition_variable.h
      base/synchronization/condition_variable_posix.cc
      base/synchronization/lock.cc
      base/synchronization/lock.h
      base/synchronization/lock_impl.h
      base/synchronization/lock_impl_posix.cc
      base/synchronization/waitable_event.h
      base/synchronization/waitable_event_posix.cc
      base/sys_info.cc
      base/sys_info.h
      base/sys_info_linux.cc
      base/sys_info_posix.cc
      base/task_runner.cc
      base/task_runner.h
      base/template_util.h
      base/third_party/dmg_fp/dmg_fp.h
      base/third_party/dmg_fp/dtoa_wrapper.cc
      base/third_party/dmg_fp/g_fmt.cc
      base/third_party/dynamic_annotations/dynamic_annotations.c
      base/third_party/dynamic_annotations/dynamic_annotations.h
      base/third_party/icu/icu_utf.cc
      base/third_party/icu/icu_utf.h
      base/third_party/nspr/prtime.cc
      base/third_party/nspr/prtime.h
      base/third_party/valgrind/memcheck.h
      base/third_party/valgrind/valgrind.h
      base/third_party/xdg_user_dirs/xdg_user_dir_lookup.cc
      base/thread_task_runner_handle.cc
      base/thread_task_runner_handle.h
      base/threading/non_thread_safe.h
      base/threading/non_thread_safe_impl.cc
      base/threading/non_thread_safe_impl.h
      base/threading/platform_thread.h
      base/threading/platform_thread_linux.cc
      base/threading/platform_thread_posix.cc
      base/threading/post_task_and_reply_impl.cc
      base/threading/post_task_and_reply_impl.h
      base/threading/sequenced_worker_pool.cc
      base/threading/sequenced_worker_pool.h
      base/threading/simple_thread.cc
      base/threading/simple_thread.h
      base/threading/thread.cc
      base/threading/thread.h
      base/threading/thread_checker.h
      base/threading/thread_checker_impl.cc
      base/threading/thread_checker_impl.h
      base/threading/thread_collision_warner.cc
      base/threading/thread_collision_warner.h
      base/threading/thread_id_name_manager.cc
      base/threading/thread_id_name_manager.h
      base/threading/thread_local.h
      base/threading/thread_local_posix.cc
      base/threading/thread_local_storage.cc
      base/threading/thread_local_storage.h
      base/threading/thread_local_storage_posix.cc
      base/threading/thread_restrictions.cc
      base/threading/thread_restrictions.h
      base/threading/worker_pool.cc
      base/threading/worker_pool_posix.cc
      base/time/time.cc
      base/time/time.h
      base/time/time_posix.cc
      base/timer/timer.cc
      base/timer/timer.h
      base/tracked_objects.cc
      base/tracked_objects.h
      base/tracking_info.cc
      base/tracking_info.h
      base/tuple.h
      base/values.cc
      base/vlog.cc
      base/vlog.h
      build/build_config.h
      third_party/superfasthash/superfasthash.c
)

set(BASE_LINK_LIBRARIES 
)

include_directories(
    ${BASE_INCLUDE_DIRECTORIES}
    ${BASE_INCLUDE_PLATFORM_DIRECTORIES}
)

add_definitions(
    ${BASE_DEFINITIONS}
)

link_directories(
    ${BASE_LINK_DIRECTORIES}
)

add_library(${BASE_TARGET_NAME} ${BASE_BUILD_CONFIGURATION_TYPE}
    ${BASE_SOURCE}
    ${BASE_PLATFORM_SOURCE}
)

target_link_libraries(${BASE_TARGET_NAME}
    ${BASE_LINK_LIBRARIES}
    ${BASE_LINK_PLATFORM_LIBRARIES}
)

set_property(TARGET ${BASE_TARGET_NAME} PROPERTY POSITION_INDEPENDENT_CODE ON)