INCLUDES = -I./include -I./external/spdlog/include -I./external/GLM/include -I./external/SDL3/include -I./external/SDL3_Image/include -I./external/nlohmann_json/include

all: build_main build_include link

build_main:
	g++ -c ./src/main.cpp \
		$(INCLUDES) \
		-o ./bin/main.o


build_include:
	g++ -c ./src/Component.cpp \
		$(INCLUDES) \
		-o ./bin/Component.o

	g++ -c ./src/Components/TransformComponent.cpp \
		$(INCLUDES) \
		-o ./bin/TransformComponent.o

	g++ -c ./src/Components/SpriteComponent.cpp \
		$(INCLUDES) \
		-o ./bin/SpriteComponent.o

	g++ -c ./src/Components/ColliderComponent.cpp \
		$(INCLUDES) \
		-o ./bin/ColliderComponent.o

	g++ -c ./src/Entity.cpp \
		$(INCLUDES) \
		-o ./bin/Entity.o

	g++ -c ./src/World.cpp \
		$(INCLUDES) \
		-o ./bin/World.o

	g++ -c ./src/ConfigLoader.cpp \
		$(INCLUDES) \
		-o ./bin/ConfigLoader.o
		
	g++ -c ./src/Game.cpp \
		$(INCLUDES) \
		-o ./bin/Game.o

	g++ -c ./src/Components/HealthComponent.cpp \
		$(INCLUDES) \
		-o ./bin/HealthComponent.o

	g++ -c ./src/Components/EnemyComponent.cpp \
		$(INCLUDES) \
		-o ./bin/EnemyComponent.o

	g++ -c ./src/Components/ObstacleComponent.cpp \
		$(INCLUDES) \
		-o ./bin/ObstacleComponent.o

	g++ -c ./src/EntityFactory.cpp \
		$(INCLUDES) \
		-o ./bin/EntityFactory.o

	g++ -c ./src/Event.cpp \
		$(INCLUDES) \
		-o ./bin/Event.o

	g++ -c ./src/Components/PlayerComponent.cpp \
		$(INCLUDES) \
		-o ./bin/PlayerComponent.o

	g++ -c ./src/Systems/MovementSystem.cpp \
		$(INCLUDES) \
		-o ./bin/MovementSystem.o

	g++ -c ./src/Systems/CollisionSystem.cpp \
		$(INCLUDES) \
		-o ./bin/CollisionSystem.o

	g++ -c ./src/Systems/CollisionResponseSystem.cpp \
		$(INCLUDES) \
		-o ./bin/CollisionResponseSystem.o

	g++ -c ./src/Systems/RenderSystem.cpp \
		$(INCLUDES) \
		-o ./bin/RenderSystem.o

	g++ -c ./src/Systems/PlayerInputSystem.cpp \
		$(INCLUDES) \
		-o ./bin/PlayerInputSystem.o

	g++ -c ./src/Systems/DamageSystem.cpp \
		$(INCLUDES) \
		-o ./bin/DamageSystem.o

	g++ -c ./src/Systems/MusicSystem.cpp \
		$(INCLUDES) \
		-o ./bin/MusicSystem.o

	g++ -c ./src/Systems/EnemyAISystem.cpp \
		$(INCLUDES) \
		-o ./bin/EnemyAISystem.o

	g++ -c ./src/Systems/SpawnSystem.cpp \
		$(INCLUDES) \
		-o ./bin/SpawnSystem.o

	g++ -c ./src/Systems/LifetimeSystem.cpp \
		$(INCLUDES) \
		-o ./bin/LifetimeSystem.o

	g++ -c ./src/Systems/HUDSystem.cpp \
		$(INCLUDES) \
		-o ./bin/HUDSystem.o

	g++ -c ./src/Systems/WaveSystem.cpp \
		$(INCLUDES) \
		-o ./bin/WaveSystem.o

	g++ -c ./src/Systems/WaveManagerSystem.cpp \
		$(INCLUDES) \
		-o ./bin/WaveManagerSystem.o

	g++ -c ./src/Systems/MusicSystem.cpp \
		$(INCLUDES) \
		-o ./bin/MusicSystem.o

link:
	g++ ./bin/*.o \
		-Lexternal/SDL3/lib \
		-Lexternal/SDL3_Image/lib \
		-Lexternal/spdlog/lib \
		-Lexternal/GLM/lib \
		-lglm \
		-lspdlog \
		-lSDL3 -lSDL3_image -lwinmm -limm32 -lversion -lole32 -loleaut32 \
		-lsetupapi -lshell32 -luser32 -lgdi32 -luuid \
		-o ./bin/main

run:
	./bin/main.exe

clean:
	rm -rf ./bin/*.o
	rm -rf ./bin/*.exe

build_static:
	g++ -c ./src/calcualadora.cpp \
		-I./include \
		-o ./bin/calculadora.o
	ar rcs ./bin/lib/libcalculadora.lib \
		./bin/calculadora.o