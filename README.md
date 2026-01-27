📘 People You May Know – A Python Recommendation Engine (Pure Python)
📌 Project Overview

People You May Know is a mini social network analytics project built using pure Python (no Pandas, NumPy, or external libraries).
The project simulates how real-world platforms like Facebook or LinkedIn recommend new friends and pages by analyzing user connections and interests.

This project demonstrates:

Data loading and cleaning

Graph-like relationship analysis

Recommendation systems using basic algorithms

Strong command over Python fundamentals

🎯 Features Implemented

✔ Load and explore social network data from JSON
✔ Clean messy and inconsistent data
✔ Recommend People You May Know using mutual friends
✔ Recommend Pages You Might Like using collaborative filtering
✔ Built entirely with core Python

🧠 Project Logic (High Level)
1️⃣ Data Loading

JSON file contains:

Users (id, name, friends, liked pages)

Pages (id, name)

Data is loaded using Python’s built-in json module.

2️⃣ Data Cleaning

The cleaning process handles:

Users with missing names

Duplicate friends

Inactive users (no friends & no liked pages)

Duplicate page IDs

This ensures reliable and structured data for analysis.

3️⃣ People You May Know

Logic:

If two users are not friends but share mutual friends, they are recommended.

More mutual friends = higher recommendation priority.

📌 Example:
If Amit and Sara share Priya as a mutual friend, Sara is recommended to Amit.

4️⃣ Pages You Might Like

Logic (Collaborative Filtering):

If two users like the same pages, they may like other pages liked by each other.

Pages liked by similar users are recommended.

Ranking is based on the number of shared interests.

🗂 Project Structure
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


(No external dependencies required)

▶️ How to Run

Clone the repository:

git clone https://github.com/your-username/people-you-may-know.git


Run any module using Python:

python src/people_you_may_know.py

🛠 Technologies Used

Python 3

JSON (built-in module only)

🚫 No Pandas
🚫 No NumPy
🚫 No external libraries

📈 Learning Outcomes

Through this project, I strengthened my understanding of:

Python data structures (lists, sets, dictionaries)

File handling & JSON parsing

Algorithmic thinking

Recommendation system fundamentals

Writing clean, modular Python code

🚀 Future Improvements

Convert logic into REST APIs

Add weighted recommendations

Integrate database support

Build a simple frontend visualization

👤 Author

Faiyaz
Computer Science & Engineering Student
Aspiring Software Engineer / Data Scientist