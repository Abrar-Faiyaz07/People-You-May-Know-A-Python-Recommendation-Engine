# Book Recommendation System (C++ | OOP | Academic Project)

## Project Overview
This is a Book Recommendation System developed using C++ and Object-Oriented Programming (OOP) concepts.  
The system manages books, users, and ratings using a file-based data storage approach and provides book recommendations through a final hybrid recommendation logic.

This project was created for academic and learning purposes.

## Team Name
The Semi-Colons

## Features
- Store book details such as title, author, genre, year, popularity, and rating
- Store user preferences and reading history
- Allow users to rate books
- Recommend books based on user interests and stored rating data
- Search books by keyword
- Filter books by genre, rating, and year
- File-based DBMS using CSV files
- Proper implementation of OOP principles

## Technologies Used
- Language: C++
- Concepts:
  - Classes and Objects
  - Encapsulation
  - Inheritance
  - Abstract Class
  - Polymorphism
  - Operator Overloading
  - Templates
  - Exception Handling
  - File Handling

## Recommendation Logic
The running system uses one final recommendation logic:
- Hybrid Recommendation

Recommendations are generated using:
- User preferred genres and authors
- User rating history
- Similar user behavior
- Book popularity as a supporting signal

## OOP Concepts Implemented
- Encapsulation:
  Data members are kept private inside classes like `Book`, `User`, and `Rating`, with controlled access through public member functions.
- Inheritance:
  Recommendation strategy classes inherit from `IRecommendationStrategy`, filter classes inherit from `BookFilter`, and custom exception classes inherit from `ProjectException`.
- Abstract Class:
  `IRecommendationStrategy` and `BookFilter` are used as abstract base classes.
- Polymorphism:
  `RecommendationEngine` works through the base strategy interface and executes the final recommendation logic dynamically.
- Operator Overloading:
  `Book` implements `operator==`, `operator<`, and `operator<<`.
- Templates:
  Template functions are used in filtering and sorting support.
- Exception Handling:
  Custom exceptions are used for validation and file access problems.

## Project Structure
```text
Book-Recommendation-System/
├── data/
│   ├── books.csv
│   ├── users.csv
│   └── ratings.csv
├── include/
│   ├── AppExceptions.h
│   ├── Book.h
│   ├── BookFilter.h
│   ├── BookRepository.h
│   ├── CollaborativeStrategy.h
│   ├── ContentBasedStrategy.h
│   ├── CSVParser.h
│   ├── HybridStrategy.h
│   ├── IRecommendationStrategy.h
│   ├── Rating.h
│   ├── RatingRepository.h
│   ├── RecommendationEngine.h
│   ├── Repository.h
│   ├── SearchService.h
│   ├── User.h
│   └── UserRepository.h
├── src/
│   ├── Book.cpp
│   ├── BookRepository.cpp
│   ├── CollaborativeStrategy.cpp
│   ├── ContentBasedStrategy.cpp
│   ├── CSVParser.cpp
│   ├── HybridStrategy.cpp
│   ├── main.cpp
│   ├── MenuController.cpp
│   ├── Rating.cpp
│   ├── RatingRepository.cpp
│   ├── RecommendationEngine.cpp
│   ├── Repository.cpp
│   ├── SearchService.cpp
│   ├── User.cpp
│   └── UserRepository.cpp
├── Makefile
├── run.bat
└── README.md
```

## How to Run
### Compile
```bash
g++ -std=c++14 -Wall -Wextra -I include src/Book.cpp src/User.cpp src/Rating.cpp src/CSVParser.cpp src/BookRepository.cpp src/UserRepository.cpp src/RatingRepository.cpp src/Repository.cpp src/ContentBasedStrategy.cpp src/CollaborativeStrategy.cpp src/HybridStrategy.cpp src/RecommendationEngine.cpp src/SearchService.cpp src/MenuController.cpp src/main.cpp -o book_recommendation.exe
```

### Run
```bash
./book_recommendation.exe
```

On Windows, you can also run:
```bat
run.bat
```

## Learning Outcomes
- Hands-on experience with OOP in C++
- File handling as a lightweight database
- Designing a recommendation system
- Using inheritance, abstraction, and polymorphism in a real project
- Team-based software development

## Developed By
The Semi-Colons
