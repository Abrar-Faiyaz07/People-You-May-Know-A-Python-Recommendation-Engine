People You May Know – A Python Recommendation Engine
Project Overview

People You May Know is a mini social network analysis project built entirely with pure Python. It simulates how social platforms like Facebook or LinkedIn suggest new friends and pages by analyzing user connections and interests using basic logic and algorithms.

This project demonstrates:

Data loading and cleaning

Analysis of user relationships

Recommendation logic based on mutual connections and shared interests

Solid understanding of Python fundamentals

Features Implemented

✅ Load and explore social network data from JSON

✅ Clean messy and inconsistent data

✅ Recommend people based on mutual friends

✅ Recommend pages based on shared interests

✅ Built entirely with core Python, no external libraries

Project Logic (High Level)
Data Loading

The JSON file contains:

Users (id, name, friends, liked pages)

Pages (id, name)

Data is loaded using Python’s built-in json module.

Data Cleaning

The cleaning process handles:

Users with missing names

Duplicate friends

Inactive users (no friends & no liked pages)

Duplicate page IDs

This ensures reliable and structured data for analysis.

People You May Know

Logic:

Recommend users who are not yet friends but share mutual friends.

More mutual friends = higher recommendation priority.

Example:
If Amit and Sara share Priya as a mutual friend, Sara is recommended to Amit.

Pages You Might Like

Logic:

Recommend pages liked by other users who share similar interests.

Pages are ranked based on the number of shared likes between users.

Project Structure
<pre>
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
</pre>


Note: No external dependencies required.

How to Run

Clone the repository:

git clone https://github.com/Abrar-Faiyaz07/People-You-May-Know-A-Python-Recommendation-Engine.git


Run any module using Python:

python src/people_you_may_know.py

Technologies Used

Python 3

JSON (built-in module only)

No external libraries like Pandas or NumPy are required.

Learning Outcomes

Strong grasp of Python data structures (lists, sets, dictionaries)

File handling & JSON parsing

Algorithmic thinking

Implementing recommendation logic without AI/ML

Writing clean, modular Python code

Future Improvements

Convert logic into REST APIs

Add weighted recommendations based on friend counts

Integrate database support

Build a simple frontend for visualization

Author

Faiyaz
Computer Science & Engineering Student
Aspiring Software Engineer / Data Scientist