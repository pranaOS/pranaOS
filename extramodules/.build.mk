ECHFS:=extramodules/echfs/echfs-utils
LIMINE:=extramodules/limine/limine-install
LIMINE_LOADER:=extramodules/limine/limine.bin

$(ECHFS):
	cd extramodules/echfs/ && $(MAKE) all

$(LIMINE):
	cd extramodules/limine/ && $(MAKE) all

