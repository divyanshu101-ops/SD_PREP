# Module 1: Core ML/DL & Evaluation Metrics (The Foundation)

> Before we look at how models break or cause financial losses in production, we must understand **how they are built, how they are evaluated, and where things usually go wrong technically.**

---

## Table of Contents

1. [Algorithm Selection: Why Choose What?](#1-algorithm-selection-why-choose-what)
2. [Overfitting vs. Underfitting](#2-overfitting-vs-underfitting-the-two-extremes)
3. [Evaluation Metrics vs. Business Loss](#3-evaluation-metrics-vs-business-loss)
4. [Quick Revision Cheat Sheet](#4-quick-revision-cheat-sheet)
5. [Common Interview Questions](#5-common-interview-questions)

---

## 1. Algorithm Selection: Why Choose What?

Different problems need different tools. Choosing the wrong model can lead to:

- **High latency** (predictions are too slow for production)
- **Poor predictions** (the model can't capture the pattern)
- **Wasted engineering effort** (weeks spent on a model that a simpler one would beat)

The first question to ask is: **What kind of data do I have?**

### 1.1 Tree-Based Models (XGBoost, Random Forest)

**When to use:** Whenever you are dealing with **tabular data**, i.e., rows and columns like a spreadsheet.

Typical examples:
- User demographics
- Transaction logs
- Predicting customer churn
- Predicting credit risk

**Why they work well:**

| Reason | Explanation |
|---|---|
| Handle non-linear relationships | Trees split data using "if-else" rules, so they naturally capture complex interactions (e.g., *"if age < 25 AND clicks > 10 → likely to buy"*). |
| Need less data cleaning | They don't require heavy feature scaling/normalization, and many implementations handle missing values well. |
| Computationally fast | Training and inference are quick, which keeps latency low in production. |

### 1.2 Deep Learning (Neural Networks / CNNs / Transformers)

**When to use:** For **unstructured data**:

- Images
- Raw text
- Audio
- Video
- Massive sequential streams

**Why:** Traditional algorithms cannot extract patterns from a raw picture or raw text. Deep learning's many layers automatically learn useful features (edges → shapes → objects in images; words → phrases → meaning in text).

### 1.3 Real-World Example: Using the Wrong Tool

Suppose you want to predict **whether a user will buy a shoe** using just `age` and `click_count` (a simple tabular e-commerce dataset).

| Choice | Outcome |
|---|---|
| Heavy Neural Network | **Overfits** (too much capacity for so little data) and **runs slowly**. |
| **XGBoost** | Fast, accurate, easy to maintain. **This is the right choice.** |

> **Rule of thumb:** *Tabular data → tree-based models. Unstructured data → deep learning.*

### 1.4 The Machine Learning Family Tree

```
                    Machine Learning
                       │
        ┌──────────────┴──────────────┐
        ↓                             ↓
   Supervised                    Unsupervised
        │                             │
   ┌────┴────┐                   ┌────┴────┐
   ↓         ↓                   ↓         ↓
Regression Classification     Clustering  PCA
   │         │
   ↓         ↓
Linear     Logistic
Regression  Regression
            Decision Tree
            Random Forest
            XGBoost
                 │
                 ↓
           Deep Learning
                 │
       ┌─────────┼─────────┐
       ↓         ↓         ↓
      CNN       RNN    Transformers
       │         │         │
     Images   Sequences   Text/LLMs
```

**How to read this diagram:**

- **Supervised learning:** The data has **labels** (the right answers). The model learns from examples.
  - **Regression:** Predict a *number* (e.g., house price) → Linear Regression.
  - **Classification:** Predict a *category* (e.g., spam / not spam) → Logistic Regression, Decision Tree, Random Forest, XGBoost.
- **Unsupervised learning:** The data has **no labels**. The model finds structure on its own.
  - **Clustering:** Group similar items together (e.g., customer segments).
  - **PCA:** Reduce the number of features while keeping the most important information.
- **Deep Learning** is a powerful branch built on neural networks:
  - **CNN** (Convolutional Neural Network): best for **images**.
  - **RNN** (Recurrent Neural Network): best for **sequences** (time series, older text models).
  - **Transformers:** best for **text and LLMs** (the architecture behind modern language models).

---

## 2. Overfitting vs. Underfitting (The Two Extremes)

Every ML model tries to find a **sweet spot** between:

- **Learning the real pattern**, and
- **Ignoring random noise**

### 2.1 Underfitting: The Model Is Too Simple

- It **fails to learn** the pattern even from the training data.
- Because it never learned the pattern, it also **fails on live data**.

**Example:** Predicting housing prices using only one useless feature, like the **owner's first name**. The name has no relationship with the price, so the model learns nothing.

**Symptoms:** High error on **both** training data and test data.

### 2.2 Overfitting: The Model Memorizes Instead of Learning

- The model learns the training data **too strictly**, including the **noise and outliers**.
- Think of a student who **memorizes past exam answers word-for-word** instead of understanding the concepts. When a brand-new question shows up, they fail.

**Example (fraud detection):**
A fraud model memorizes that *"every user named John who buys a laptop at 3 AM is a fraudster."*
Later, a **legitimate** user named John buys a laptop at 3 AM → the model **blocks him**. This is a **False Positive**.

**Symptoms:** **Very low** error on training data, but **high** error on new/test data.

### 2.3 Side-by-Side Comparison

| | Underfitting | Good Fit | Overfitting |
|---|---|---|---|
| Model complexity | Too low | Just right | Too high |
| Training error | High | Low | Very low |
| Test/production error | High | Low | High |
| Analogy | Student who didn't study | Student who understood concepts | Student who memorized answers |
| Also called | High **bias** | Balanced | High **variance** |

### 2.4 How to Reduce Each Problem (Useful Extras)

| Problem | Common Fixes |
|---|---|
| Underfitting | Use a more complex model, add better features, train longer, reduce regularization. |
| Overfitting | Get more data, simplify the model, use regularization, use cross-validation, early stopping, dropout (for neural nets). |

---

## 3. Evaluation Metrics vs. Business Loss

> **This is where interviews are won or lost.**
> Standard metrics like **Accuracy** can lie to you. You must look at **Precision** and **Recall** and map them directly to **money and business impact**.

### 3.1 The Foundation: The Confusion Matrix

Every metric below comes from four numbers:

|  | **Predicted Positive** | **Predicted Negative** |
|---|---|---|
| **Actually Positive** | **TP** (True Positive): correctly caught | **FN** (False Negative): missed! |
| **Actually Negative** | **FP** (False Positive): false alarm! | **TN** (True Negative): correctly ignored |

- **False Positive (FP):** The model says "yes", but reality is "no". *(A false alarm.)*
- **False Negative (FN):** The model says "no", but reality is "yes". *(A missed real case.)*

### 3.2 Why Accuracy Can Lie

Accuracy = (TP + TN) / Total predictions.

**Example:** Out of 10,000 transactions, only 10 are fraud. A useless model that says **"everything is safe"** is right 9,990 times → **99.9% accuracy**, yet it catches **zero** fraud.

This is why, on **imbalanced data** (fraud, disease, churn), accuracy is misleading, and we use Precision and Recall instead.

### 3.3 Precision: Focus on False Positives

**Question it answers:** *Out of everything the model predicted as positive, how many were actually positive?*

```
Precision = TP / (TP + FP)
```

**Business meaning:** **High precision = fewer false alarms.**

**Example: Email Spam Filter**

- A **False Positive** means an **important job-interview email lands in the spam folder**.
- That could cost someone their career opportunity.
- **Therefore, high precision is mandatory** here. We'd rather let a few spam emails through than hide an important one.

### 3.4 Recall: Focus on False Negatives

**Question it answers:** *Out of all the actual positives in reality, how many did the model catch?*

```
Recall = TP / (TP + FN)
```

**Business meaning:** **High recall = you miss as few real threats/opportunities as possible.**

**Example: Cancer Detection or Credit Card Fraud**

- A **False Negative** means the model says a transaction is safe, but it is **actually fraud**. The bank **loses $10,000**.
- In cancer detection, a False Negative means a sick patient is told they are healthy.
- **Therefore, high recall is mandatory** here. We'd rather investigate a few extra harmless cases than miss a real one.

### 3.5 Precision vs. Recall: The Trade-off

You usually **can't maximize both at once**. Models output a probability, and you choose a **threshold** to decide "positive" vs "negative":

| If you... | Precision | Recall |
|---|---|---|
| **Raise** the threshold (be stricter) | ↑ goes up | ↓ goes down |
| **Lower** the threshold (be more lenient) | ↓ goes down | ↑ goes up |

So the real question is always: **Which mistake is more expensive for the business?**

### 3.6 Mapping Metrics to Money

| Scenario | Costlier Error | Metric to Prioritize | Why |
|---|---|---|---|
| Spam filter | False Positive (real email hidden) | **Precision** | Losing an important email is worse than seeing some spam. |
| Credit card fraud | False Negative (fraud missed) | **Recall** | A missed fraud directly loses money (e.g., $10,000). |
| Cancer detection | False Negative (sick patient missed) | **Recall** | A missed diagnosis can cost a life. |

> **Important nuance:** Fraud detection also has a cost on the precision side (blocking legitimate customers, like "John" in Section 2). The best answer in an interview always **acknowledges the trade-off** and explains *which side the business can tolerate more*.

### 3.7 Bonus: F1 Score

When you need **one number that balances both**, use the F1 Score (harmonic mean of Precision and Recall):

```
F1 = 2 × (Precision × Recall) / (Precision + Recall)
```

It is low if **either** precision or recall is low, so it can't be "gamed" by excelling at only one.

### 3.8 Worked Example

A fraud model on 1,000 transactions produces:

- TP = 40 (fraud caught)
- FP = 10 (legit transactions wrongly blocked)
- FN = 20 (fraud missed)
- TN = 930 (legit, correctly allowed)

```
Precision = 40 / (40 + 10) = 0.80   → 80% of alerts were real fraud
Recall    = 40 / (40 + 20) = 0.67   → we caught 67% of all fraud
Accuracy  = (40 + 930) / 1000 = 97% → looks great, but hides the 20 missed frauds!
```

---

## 4. Quick Revision Cheat Sheet

| Topic | One-Line Takeaway |
|---|---|
| Tabular data | Use **XGBoost / Random Forest** (fast, accurate, less cleaning). |
| Unstructured data | Use **Deep Learning** (CNN for images, Transformers for text). |
| Underfitting | Model too simple, fails on train **and** test. |
| Overfitting | Model memorizes noise, great on train, bad on new data. |
| Accuracy | Can be misleading on imbalanced data. |
| Precision | Of predicted positives, how many were right? → controls **False Positives**. |
| Recall | Of actual positives, how many did we catch? → controls **False Negatives**. |
| Choosing a metric | Ask: **"Which error costs the business more?"** |

---

## 5. Common Interview Questions

1. **Why would you choose XGBoost over a neural network for a churn prediction problem?**
   *Churn data is tabular. Tree-based models handle non-linear relationships well, need less cleaning, train and predict faster, and are less likely to overfit on small structured datasets.*

2. **What is overfitting, and how would you detect it in production?**
   *The model memorizes training noise. Detect it by a big gap between training performance and validation/production performance; reduce it with more data, regularization, cross-validation, or a simpler model.*

3. **Your fraud model has 99.9% accuracy. Is it good?**
   *Not necessarily. Fraud is rare, so a model predicting "no fraud" always would also score ~99.9%. Check precision, recall, and F1.*

4. **For a spam filter, do you optimize precision or recall? What about fraud detection?**
   *Spam → precision (avoid hiding real emails). Fraud → recall (avoid missing real fraud), while monitoring precision to avoid blocking legitimate customers.*

5. **How do you convert Precision/Recall into business terms?**
   *Assign a cost to each error type (cost of an FP vs. cost of an FN), then choose the threshold that minimizes total expected loss.*

---

*End of Module 1.*
