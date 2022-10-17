FROM debian:stable-slim

WORKDIR /app

RUN apt-get update && apt-get install -y \
	cmake \
	make \
	gcc \
	googletest \
	g++ \
	python3-pip \
	valgrind \
	clang-format \
	clang-tidy

# GoogleTestsのインストール
# REF: https://stackoverflow.com/questions/24295876/cmake-cannot-find-googletest-required-library-in-ubuntu
RUN (cd /usr/src/googletest && cmake . && cmake --build . --target install)

# cpplintのインストール
RUN pip install cpplint

# COPY ./tools/start.sh /usr/local/bin
# prevent container from terminating
ENTRYPOINT ["tail", "-f", "/dev/null"]
