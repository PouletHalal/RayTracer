##
## EPITECH PROJECT, 2024
## Makefile
## File description:
## Makefile cleaned by the code review
##

NAME =		raytracer

SRC =		src/main.cpp			\
			src/Vector.cpp			\
			src/Sphere.cpp			\
			src/Camera.cpp			\
			src/Ray.cpp				\
			src/HitRecord.cpp		\
			src/Scene.cpp			\
			src/Rectangle.cpp		\
			src/Plane.cpp			\
			src/Triangle.cpp		\
			src/Utils.cpp			\
			src/AABB.cpp			\
			src/AShape.cpp			\
			src/Material.cpp

OBJ =		$(SRC:.cpp=.o)

CFLAGS =	-Wall -Wextra -lsfml-graphics -lsfml-window -lsfml-system

CPPFLAGS =	-iquote ./include -g

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) -std=c++20 -o $(NAME) $(OBJ) $(CPPFLAGS) $(CFLAGS)

test: all
	./$(NAME)

clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY = all, clean, fclean, re
