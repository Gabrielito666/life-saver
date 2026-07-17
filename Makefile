CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
SRC_DIR = src
BIN_DIR = bin
SRC = $(SRC_DIR)/main.c $(SRC_DIR)/parse-args.c $(SRC_DIR)/command_help.c $(SRC_DIR)/list-items.c $(SRC_DIR)/pack.c $(SRC_DIR)/command-saver.c
TARGET = $(BIN_DIR)/life-saver

.PHONY: build clean install uninstall debian lab test

build:
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)
	@echo "Build complete: $(TARGET)"

install: build
	sudo install -d $(DESTDIR)/usr/bin
	sudo install -m 755 $(TARGET) $(DESTDIR)/usr/bin/life-saver

uninstall:
	sudo rm -f $(DESTDIR)/usr/bin/life-saver

test:
	docker build -t life-saver-test -f docker/Dockerfile.test .
	docker run --rm life-saver-test
	docker rmi life-saver-test

clean:
	rm -rf $(BIN_DIR)
	docker rmi life-saver-lab 2>/dev/null || true
	docker rmi life-saver-test 2>/dev/null || true
	@echo "Cleaned bin/ and docker images"

debian:
	dpkg-buildpackage -us -uc -b
	mv ../life-saver_*.deb debian/

lab: build debian
	docker build -t life-saver-lab -f docker/Dockerfile.lab .
	docker run --rm -it life-saver-lab
	docker rmi life-saver-lab
