```markdown
# People You May Know – A Python Recommendation Engine

## Project Overview

People You May Know is a mini social network analysis project built entirely using core Python. The project simulates how platforms such as Facebook or LinkedIn recommend new friends and pages by analyzing user connections and shared interests through logical algorithms.

The goal of this project is to demonstrate strong fundamentals in Python, data handling, and algorithmic thinking without relying on external libraries.

---

## Key Objectives

- Load and process structured social network data
- Clean inconsistent and incomplete datasets
- Analyze user relationships
- Implement recommendation logic based on mutual connections and shared interests
- Apply modular and maintainable Python design

---

## Features Implemented

- Load and explore social network data from JSON files
- Clean messy or inconsistent data
- Recommend people based on mutual friends
- Recommend pages based on shared interests
- Built entirely using core Python (no external libraries)

---

## High-Level Project Logic

### 1. Data Loading

The dataset is stored in JSON format and contains:

- Users (`id`, `name`, `friends`, `liked_pages`)
- Pages (`id`, `name`)

Data is loaded using Python’s built-in `json` module.

---

### 2. Data Cleaning

The cleaning process ensures data reliability by handling:

- Users with missing or empty names
- Duplicate friend IDs
- Inactive users (no friends and no liked pages)
- Duplicate page IDs

This step guarantees structured and consistent input for recommendation algorithms.

---

### 3. People You May Know

**Logic:**

- Recommend users who are not already friends
- Identify shared mutual friends
- Prioritize users with a higher number of mutual connections

**Example:**

If Amit and Sara both share Priya as a mutual friend, Sara may be recommended to Amit.

---

### 4. Pages You Might Like

**Logic:**

- Analyze pages liked by users with similar interests
- Recommend pages not already liked by the current user
- Rank pages based on the frequency of shared interests

---

## Project Structure

```

people-you-may-know/
│
├── data/
│   ├── codebook_data.json
│   ├── cleaned_codebook_data.json
│
├── src/
│   ├── load_data.py
│   ├── clean_data.py
│   ├── people_you_may_know.py
│   ├── pages_you_might_like.py
│
├── README.md
└── requirements.txt

````

Note: No external dependencies are required.

---

## How to Run

Clone the repository:

```bash
git clone https://github.com/Abrar-Faiyaz07/People-You-May-Know-A-Python-Recommendation-Engine.git
````

Navigate to the project directory:

```bash
cd People-You-May-Know-A-Python-Recommendation-Engine
```

Run any module using Python:

```bash
python src/people_you_may_know.py
```

---

## Technologies Used

* Python 3
* JSON (built-in module only)

No third-party libraries such as Pandas or NumPy are used.

---

## Learning Outcomes

* Strong understanding of Python data structures (lists, sets, dictionaries)
* File handling and JSON parsing
* Algorithmic problem-solving
* Designing recommendation logic without machine learning libraries
* Writing modular, maintainable Python code

---

## Future Improvements

* Introduce weighted scoring for recommendations
* Add database integration
* Expose functionality via REST APIs
* Build a simple frontend for visualization
* Implement ranking optimization strategies

---

## Author

Faiyaz
Computer Science & Engineering Student
Aspiring Software Engineer / Data Scientist

```
```
