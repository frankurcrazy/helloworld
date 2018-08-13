CC = gcc
APP_NAME = akav1-md5


# ======================
#  Source files
# ======================

SRC  = akav1-md5.c 
SRC += 3gpp_aka.c
SRC += base64.c
SRC += md5.c
SRC += digcalc.c


##################
# CFLAGS
##################

CFLAGS  = -Wall


##################
# LDFLAGS
##################

LDFLAGS  = -Wall


DEP_CFLAGS = $(filter-out -g, $(CFLAGS))
DEPEND_CMD = $(CC) $(DEP_CFLAGS) -M $<


#################
# Apps
#################

OBJS = $(SRC:.c=.o)
DEPS = $(SRC:.c=.c.d)


all: $(APP_NAME)
	@echo "Build ... $(APP_NAME)"

-include $(DEPS)

$(APP_NAME): $(OBJS)
	$(CC) $(LDFLAGS) $(OBJS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

ifneq ($(MAKECMDGOALS), clean)
%.c.d: %.c
	@echo Generating depend file $(subst $(BASE_DIR)/,,$(@))
	@echo "$@ \\"        >  $@
	@echo -n "$(dir $<)" >> $@
	@$(DEPEND_CMD)       >> $@ || $(RM) $@
endif

clean:
	@echo "Clean ... $(APP_NAME)"
	@rm -f $(APP_NAME) $(OBJS) $(DEPS)

.PHONY: all clean
