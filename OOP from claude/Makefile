CXX      = g++
CXXFLAGS = -std=c++14 -Wall -Wextra -I include
SRCS     = src/Book.cpp src/User.cpp src/Rating.cpp \
           src/CSVParser.cpp \
           src/BookRepository.cpp src/UserRepository.cpp src/RatingRepository.cpp \
           src/Repository.cpp \
           src/ContentBasedStrategy.cpp src/CollaborativeStrategy.cpp src/HybridStrategy.cpp \
           src/RecommendationEngine.cpp src/SearchService.cpp \
           src/MenuController.cpp src/main.cpp
TARGET   = book_recommendation.exe

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)

clean:
	-del /Q $(TARGET)
