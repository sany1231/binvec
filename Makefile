MODULES = binvec
EXTENSION = binvec
EXTENSION_VERSION = 0.1
DATA = binvec--$(EXTENSION_VERSION).sql

PG_CONFIG = pg_config
PGXS := $(shell $(PG_CONFIG) --pgxs)
include $(PGXS)
