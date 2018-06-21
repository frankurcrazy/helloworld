CC = gcc
APP_NAME = crc_test


# ======================
#  Source files
# ======================

SRC += main.c 
SRC += crc32.c


##################
# CFLAGS
##################

CFLAGS = -Wall


##################
# LDFLAGS
##################

LDFLAGS = -Wall


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

%.c.d: %.c
	@echo Generating depend file $(subst $(BASE_DIR)/,,$(@))
	@echo "$@ \\" > $@
	@echo -n "$(dir $<)" >> $@
	@$(DEPEND_CMD) >> $@ || $(RM) $@

clean:
	@echo "Clean ... $(APP_NAME)"
	@rm -f $(APP_NAME) $(OBJS) $(DEPS)

.PHONY: all clean
