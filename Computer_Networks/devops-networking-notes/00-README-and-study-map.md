# Networking + DevOps Interview Notes

Concept-only, no numericals. Written for backend / system design interviews.

## How to use these notes

1. Read the phases **in order**. Later phases assume earlier ones (Kubernetes networking makes no sense without IP, NAT, DNS and load balancing).
2. In every file, read the **analogy first**, then the mechanism, then the real-world example. If you can explain the analogy to a friend, you understand the concept.
3. At the end of every file there is an **Interview Q&A** section and a **Cheat Sheet**. Cover the answers and try to speak them out loud.
4. Anything marked with a star (⭐) is asked very frequently in interviews.

## Study map

| Phase | Topic | Files | Status |
|---|---|---|---|
| 1 | Networking Foundation | 3 | Batch 1 |
| 2 | Routing | 2 | Batch 1 |
| 3 | Transport + Application Layer | 3 | Batch 2 |
| 4 | Networking for System Design + Security | 2 | Batch 2 |
| 5 | DevOps Foundation (Linux, Git) | 2 | Batch 3 |
| 6 | Docker | 2 | Batch 3 |
| 7 | CI/CD | 1 | Batch 4 |
| 8 | Kubernetes | 2 | Batch 4 |
| 9 | IaC + Cloud | 2 | Batch 5 |
| 10 | Observability, Reliability, DevSecOps | 2 | Batch 5 |

## The one master analogy used across Phase 1 and 2

Think of the internet as the **postal system of a giant country**:

- A **message** you send is a **letter**.
- **Layers** are the different departments (writing, packing, sorting, transport).
- **IP address** = postal address of a building. **MAC address** = the name on the door of the exact flat.
- **Router** = a sorting office that decides which city the parcel goes to next.
- **Switch** = the lift/lobby inside one building that delivers to the right flat.
- **DNS** = the phone book that converts a name ("Google") into an address.
- **TCP** = registered post with delivery confirmation. **UDP** = dropping a postcard in a mailbox.

Keep coming back to this picture whenever a concept feels abstract.

## Golden rule for interviews

Never just define. Always follow this shape:

**What it is → Why it exists (what problem it solves) → How it works → Where it is used in the real world → Trade-offs.**
