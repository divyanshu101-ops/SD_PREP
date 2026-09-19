
1. Algorithm vs. Model
Algorithm: The mathematical formula or logic used to learn patterns from data (e.g., Linear Regression, Random Forest, or a Transformer architecture).

Model: The final output or artifact produced after training an algorithm on actual data. It is what you deploy to production to make live predictions.

2. Features vs. Target (Label)
Features: The input variables or data points that the model looks at to make a decision (e.g., a customer's age, past purchase history, or click frequency).

Target (Label): The specific output or answer you want the model to predict (e.g., whether a customer will churn or buy a product, denoted as 1 or 0).

3. Training Data vs. Inference Data
Training Data: The historical data used to teach the model how to recognize patterns.

Inference Data (Production Data): The live, real-time data streaming in from actual users that the model evaluates to generate new predictions.

4. Overfitting vs. Underfitting
Overfitting: When a model learns the training data too well (including the noise and random errors), causing it to fail completely on new, unseen live data.

Underfitting: When a model is too simple to capture the underlying patterns in the data, resulting in poor performance on both training and live data.

5. Precision vs. Recall
Precision: Out of all the positive predictions the model made, how many were actually correct? (Crucial when False Positives are costly—like falsely accusing a legitimate transaction of being fraud).

Recall: Out of all the actual positive cases in reality, how many did the model manage to catch? (Crucial when False Negatives are dangerous—like failing to detect a real fraudster or a sick patient).

6. Data Drift vs. Concept Drift
Data Drift: A change in the distribution or pattern of the input features over time (e.g., user purchasing behavior changes due to a new market trend).

Concept Drift: A change in the underlying relationship between the input features and the target (e.g., economic conditions change, so features that used to indicate a safe loan applicant now indicate a high risk).

7. RAG (Retrieval-Augmented Generation)
An architecture used in Generative AI where an LLM does not rely solely on its internal memory. Instead, it first searches a Vector Database for relevant company documents, retrieves them, and uses that context to generate an accurate, hallucination-free answer.

8. Token & Inference Cost
Token: The fundamental unit of text processed by LLMs (roughly 4 characters or 0.75 words).

Inference Cost: The financial cost incurred every time an AI model or LLM API processes a request and generates a response. High query volumes with unoptimized prompts can cause inference costs to skyrocket, turning a profitable product into a financial loss.