# Python Environment Setup - Full AI System Analysis

This folder contains the complete environment setup for working through **Full AI System Analysis** (Core ML/DL, MLOps, RAG/GenAI, and Token Economics).

---

## Environment Details

- **Location:** `Full_AI_System_Analysis/.venv`
- **Python Version:** 3.13+
- **Requirements File:** `requirements.txt`

---

## How to Activate the Environment

### PowerShell (Windows)
```powershell
.\.venv\Scripts\Activate.ps1
```

### Command Prompt (cmd)
```cmd
.\.venv\Scripts\activate.bat
```

### Git Bash / Bash
```bash
source .venv/Scripts/activate
```

---

## Managing Packages

To install or update dependencies:
```powershell
.\.venv\Scripts\pip install -r requirements.txt
```

---

## Verifying the Setup

Run the verification script:
```powershell
.\.venv\Scripts\python.exe test_env.py
```

---

## Using with Jupyter Notebooks / VS Code / Antigravity IDE

1. Open any `.ipynb` file in this workspace.
2. Select kernel -> Select another kernel -> Python Environments -> Choose `.venv\Scripts\python.exe` from `Full_AI_System_Analysis`.
