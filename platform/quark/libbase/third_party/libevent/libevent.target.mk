# This file is generated by gyp; do not edit.

TOOLSET := target
TARGET := libevent
DEFS_Debug := \
	'-D_FILE_OFFSET_BITS=64' \
	'-DHAVE_CONFIG_H'

# Flags passed to all source files.
CFLAGS_Debug := \
	-fPIC \
	-fno-exceptions \
	-fno-strict-aliasing \
	-fstack-protector-all \
	-fvisibility=hidden \
	-g \
	-pipe \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-mfpmath=sse \
	-Wno-sign-compare \
	-Wno-unused-but-set-variable \
	-O0

# Flags passed to only C files.
CFLAGS_C_Debug :=

# Flags passed to only C++ files.
CFLAGS_CC_Debug := \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-std=gnu++11 \
	-Wsign-compare

INCS_Debug := \
	-Ithird_party/libevent/linux

DEFS_Release := \
	'-D_FILE_OFFSET_BITS=64' \
	'-DHAVE_CONFIG_H' \
	'-DNDEBUG'

# Flags passed to all source files.
CFLAGS_Release := \
	-fPIC \
	-fno-exceptions \
	-fno-strict-aliasing \
	-fstack-protector-all \
	-fvisibility=hidden \
	-g \
	-pipe \
	-pthread \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-Wno-missing-field-initializers \
	-mfpmath=sse \
	-Wno-sign-compare \
	-Wno-unused-but-set-variable \
	-O3 \
	-fdata-sections \
	-ffunction-sections \
	-fno-ident

# Flags passed to only C files.
CFLAGS_C_Release :=

# Flags passed to only C++ files.
CFLAGS_CC_Release := \
	-fno-rtti \
	-fno-threadsafe-statics \
	-fvisibility-inlines-hidden \
	-std=gnu++11 \
	-Wsign-compare

INCS_Release := \
	-Ithird_party/libevent/linux

OBJS := \
	$(obj).target/$(TARGET)/third_party/libevent/buffer.o \
	$(obj).target/$(TARGET)/third_party/libevent/evbuffer.o \
	$(obj).target/$(TARGET)/third_party/libevent/evdns.o \
	$(obj).target/$(TARGET)/third_party/libevent/event.o \
	$(obj).target/$(TARGET)/third_party/libevent/event_tagging.o \
	$(obj).target/$(TARGET)/third_party/libevent/evrpc.o \
	$(obj).target/$(TARGET)/third_party/libevent/evutil.o \
	$(obj).target/$(TARGET)/third_party/libevent/http.o \
	$(obj).target/$(TARGET)/third_party/libevent/log.o \
	$(obj).target/$(TARGET)/third_party/libevent/poll.o \
	$(obj).target/$(TARGET)/third_party/libevent/select.o \
	$(obj).target/$(TARGET)/third_party/libevent/signal.o \
	$(obj).target/$(TARGET)/third_party/libevent/strlcpy.o \
	$(obj).target/$(TARGET)/third_party/libevent/epoll.o

# Add to the list of files we specially track dependencies for.
all_deps += $(OBJS)

# CFLAGS et al overrides must be target-local.
# See "Target-specific Variable Values" in the GNU Make manual.
$(OBJS): TOOLSET := $(TOOLSET)
$(OBJS): GYP_CFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_C_$(BUILDTYPE))
$(OBJS): GYP_CXXFLAGS := $(DEFS_$(BUILDTYPE)) $(INCS_$(BUILDTYPE))  $(CFLAGS_$(BUILDTYPE)) $(CFLAGS_CC_$(BUILDTYPE))

# Suffix rules, putting all outputs into $(obj).

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(srcdir)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

# Try building from generated source, too.

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj).$(TOOLSET)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

$(obj).$(TOOLSET)/$(TARGET)/%.o: $(obj)/%.c FORCE_DO_CMD
	@$(call do_cmd,cc,1)

# End of this set of suffix rules
### Rules for final target.
LDFLAGS_Debug := \
	-fPIC \
	-pthread \
	-Wl,--as-needed \
	-Wl,-z,noexecstack

LDFLAGS_Release := \
	-fPIC \
	-pthread \
	-Wl,--as-needed \
	-Wl,-z,noexecstack \
	-Wl,-O1 \
	-Wl,--gc-sections

LIBS :=

$(obj).target/third_party/libevent/libevent.a: GYP_LDFLAGS := $(LDFLAGS_$(BUILDTYPE))
$(obj).target/third_party/libevent/libevent.a: LIBS := $(LIBS)
$(obj).target/third_party/libevent/libevent.a: TOOLSET := $(TOOLSET)
$(obj).target/third_party/libevent/libevent.a: $(OBJS) FORCE_DO_CMD
	$(call do_cmd,alink_thin)

all_deps += $(obj).target/third_party/libevent/libevent.a
# Add target alias
.PHONY: libevent
libevent: $(obj).target/third_party/libevent/libevent.a

# Add target alias to "all" target.
.PHONY: all
all: libevent

