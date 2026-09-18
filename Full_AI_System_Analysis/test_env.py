"""
Environment Verification Script for Full AI System Analysis
"""

import sys
import importlib

print(f"Python Executable: {sys.executable}")
print(f"Python Version: {sys.version}\n")

packages = [
    "numpy",
    "pandas",
    "scipy",
    "sklearn",
    "xgboost",
    "matplotlib",
    "seaborn",
    "openai",
    "tiktoken",
    "faiss",
    "sentence_transformers",
    "dotenv",
    "pydantic",
    "jupyter",
]

print("Checking installed packages:")
print("=" * 40)

all_ok = True
for pkg in packages:
    try:
        mod = importlib.import_module(pkg)
        version = getattr(mod, "__version__", "installed")
        print(f"  [OK] {pkg:<22} (v{version})")
    except ImportError as e:
        print(f"  [MISSING] {pkg:<22} - {e}")
        all_ok = False

print("=" * 40)
if all_ok:
    print("All core dependencies loaded successfully!")
else:
    print("Some dependencies are missing or still installing.")
