all:
	@g++ main.cpp -o app -lGL -lglut

.PHONY: run
run:
	@g++ main.cpp -o app -lGL -lglut
	@./app

.PHONY: test
test:
	@echo There is nothing to test in this project

.PHONY: pull
pull:
	@echo There is nothing to pull in this project

.PHONY: clean
clean:
	@rm -rf app