#file searching path

ifeq ($(cpu_ssd222d), yes)
    CROSS_COMPILE=arm-none-linux-gnueabihf-
else
    CROSS_COMPILE=mips-linux-gnu-
endif

CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
AR=$(CROSS_COMPILE)ar
RM=rm
CP=cp
MKBIN =$(SDK_INSTALL_DIR)/tools/BuildBin/BuildBin.exe

LIBPATH = $(SDK_LIB_DIR)
APIPATH = $(SDK_API_DIR)
LIBAPIPATH = libapi
OBJPATH = obj_err
SRCPATH = src
INCPATH = inc
BINPATH = bin
MKNLDPATH = bin
LIBJSONINC = libcJSON/inc
LIBJSONSRC = libcJSON/src

# 程序编译参数
CFLAGS+= -W -Wall
DEFS = -DCONFIG_CPU_SSD222D

ifeq ($(cpu_ssd222d), yes)
CFLAGS+= $(DEFS)
endif

# 程序链接参数
ifeq ($(cpu_ssd222d), yes)
LDFLAGS = -L$(LIBPATH) -L$(SDK_SYS_LIB_DIR) -lsdk  -lfreetype -lpng16 -lturbojpeg -ljpeg -lz -lcjson -lpthread -lsqlite3 -lscos 
else
LDFLAGS = -L$(LIBPATH) -L$(SDK_SYS_LIB_DIR) -lsdk  -lfreetype -lpng16 -lturbojpeg -ljpeg -lz -lcjson -lpthread -lsqlite3 -lCUDW -lcurl
endif


#文件搜索路径
VPATH = $(SRCPATH) $(APIPATH) $(BINPATH) $(OBJPATH) $(LIBAPIPATH) $(SYSLIB_PATH) $(LIBJSONSRC)

#头文件搜索路径
INCLPATH = -I$(INCPATH) -I$(APIPATH) -I$(LIBAPIPATH) -I$(LIBJSONINC)


# 生成文件的路径
BINDIR = $(BINPATH)

# 生成的程序名
#factorytest.bin
NAME = tmpl
LIBFILE = 

# 程序中用到的模块
SRCS=$(wildcard $(SRCPATH)/*.c $(LIBJSONSRC)/*.c)
SRSS=$(notdir $(SRCS))
OBJS=$(subst .c,.o,$(SRSS))

OBJSD=		$(addprefix $(OBJPATH)/,$(SRSS))
OBJSY=		$(subst .c,.o,$(OBJSD))

SRCSSS=		$(wildcard $(APIPATH)/*.c)
SRSSSS=		$(notdir $(SRCSSS))
OBJSSS=		$(subst .c,.o,$(SRSSSS))

OBJSSSD=	$(subst $(APIPATH)/,$(OBJPATH)/,$(SRCSSS))
OBJSSSY=	$(subst .c,.o,$(OBJSSSD)) 

OBJS +=		$(OBJSSS)

#包含依赖文件
.all: .config $(NAME)

$(NAME):$(OBJS) $(OBJSSS)
	$(CC) -o $(BINDIR)/$(NAME) $(OBJSY) $(OBJSSSY)  $(LDFLAGS) >$(OBJPATH)/link.err -g -funwind-tables -rdynamic -Wl,-Map=$(NAME).map 
#$(MKBIN) -o $(BINDIR)/FactoryTest.tpk -t $(BINDIR)/bintp.p1t -f $(BINDIR)/tpkfilelist.txt

#自动生成依赖文件
.config: $(subst .o,.deps,$(OBJS)) 

%.o: %.c 
	$(CC) -c  $(CFLAGS) $(INCLPATH) $< -o  $(OBJPATH)/$(subst .c,.o,$@) >$(OBJPATH)/$(subst .o,.err,$@)
	
%.deps: %.c
	$(CC) -M $(INCLPATH) $< > $(OBJPATH)/$@ 

clean:
	-$(RM) -rf $(OBJPATH)/*.o $(OBJPATH)/*.deps

