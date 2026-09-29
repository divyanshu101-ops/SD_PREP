# Phase 2 · File 1: Routing Fundamentals and Algorithms

> **Goal:** understand what routing is, how a router picks the next hop, the anatomy of a routing table, and the two big families of routing algorithms (Distance Vector and Link State) along with their classic problems (count-to-infinity, loops) and fixes.

**Master analogy: GPS navigation for packets.**
- A **router** is a road junction with a signboard that says "for Mumbai take left, for Delhi go straight".
- The **routing table** is that signboard.
- **Routing protocols** are how neighbouring junctions talk to each other to keep the signboards correct, even when a road is closed.

---

## 1. Routing vs Forwarding vs Switching ⭐

| Term | Meaning | Speed | Analogy |
|---|---|---|---|
| **Routing** | Building the map: deciding *which path* to use, exchanging info with other routers | Slow (control plane, seconds) | Planning the route on a map app |
| **Forwarding** | Moving each packet out of the correct interface using the table | Very fast (data plane, hardware, nanoseconds) | Actually driving through the junction |
| **Switching (L2)** | Delivering frames inside one LAN using MAC addresses | Fast | Delivering to the right flat inside a building |

### Control plane vs Data plane
- **Control plane:** routing protocols (OSPF, BGP) run here, computing and updating the routing table.
- **Data plane (forwarding plane):** ASIC hardware that forwards packets using the pre-computed table (FIB).
- **Management plane:** how admins configure the device (SSH, SNMP).
- **SDN** separates the control plane from devices and centralizes it in a controller.

---

## 2. What a Router Does

A router:
1. Receives a packet on an interface.
2. Removes the Layer 2 frame, reads the **destination IP**.
3. Looks it up in the **routing table** (longest prefix match).
4. **Decrements TTL** (drops if 0, sends ICMP Time Exceeded).
5. Finds the **next hop** and outgoing interface, uses ARP to find the next hop's MAC.
6. Builds a **new Layer 2 frame** and sends it out.

Routers connect different networks and **do not forward broadcasts**, so each interface is a separate broadcast domain.

---

## 3. Anatomy of a Routing Table ⭐

Each entry contains:

| Field | Meaning |
|---|---|
| **Destination network / prefix** | e.g., `10.1.2.0/24` |
| **Next hop** | IP of the next router to send to |
| **Outgoing interface** | e.g., `eth0` |
| **Metric** | "Cost" of the route (hop count, bandwidth-based cost, etc.) |
| **Route source / protocol** | Connected (C), Static (S), RIP (R), OSPF (O), BGP (B) |
| **Administrative Distance** | Trustworthiness of the source of the route |

Example (Linux `ip route`):
```
default via 192.168.1.1 dev wlan0
192.168.1.0/24 dev wlan0 proto kernel scope link src 192.168.1.10
10.8.0.0/24 via 192.168.1.5 dev wlan0
```
Reading it: anything in 192.168.1.0/24 is directly connected; 10.8.0.0/24 goes via router 192.168.1.5; everything else goes to the default gateway 192.168.1.1.

### How do routes get into the table?
1. **Directly connected networks:** automatically, when an interface is configured and up.
2. **Static routes:** manually configured by an admin.
3. **Dynamic routes:** learned from routing protocols.
4. **Default route (`0.0.0.0/0`):** "if nothing else matches, send here". Used by nearly every host and edge router.

---

## 4. Longest Prefix Match ⭐⭐

If multiple table entries match a destination, the router picks the **most specific** one, meaning the one with the **longest prefix length**.

Routing table:
```
10.0.0.0/8      -> next hop A
10.1.0.0/16     -> next hop B
10.1.2.0/24     -> next hop C
0.0.0.0/0       -> next hop D (default)
```
| Destination | Matches | Chosen |
|---|---|---|
| 10.1.2.55 | /8, /16, /24, /0 | **/24 → C** (most specific) |
| 10.1.9.9 | /8, /16, /0 | **/16 → B** |
| 10.200.1.1 | /8, /0 | **/8 → A** |
| 8.8.8.8 | /0 only | **D (default)** |

**Analogy:** If you ask for "Flat 101, Block C, Sunrise Society, Kolkata", the guard uses the most detailed matching entry he knows (Sunrise Society Block C) instead of the vague "Kolkata".

This rule is what makes **route aggregation** and **BGP hijacks via more specific prefixes** possible (see file 2).

---

## 5. Metric and Administrative Distance ⭐

### Metric
Used to compare routes **learned through the same protocol**. Lower is better.
- RIP: hop count.
- OSPF: cost (based on link bandwidth).
- EIGRP: composite (bandwidth, delay).
- BGP: uses attributes/policy, not a simple metric.

### Administrative Distance (AD)
Used to compare routes **learned from different sources** for the same prefix. Lower AD = more trusted. (Cisco default values, good to know approximately.)

| Source | AD |
|---|---|
| Directly connected | 0 |
| Static route | 1 |
| eBGP | 20 |
| EIGRP | 90 |
| OSPF | 110 |
| IS-IS | 115 |
| RIP | 120 |
| iBGP | 200 |

So if both OSPF and RIP know a path to the same network, OSPF wins because 110 < 120, regardless of their metrics.

**Order of decision:** Longest prefix match → lowest AD → lowest metric.

---

## 6. Static vs Dynamic Routing ⭐

| | Static | Dynamic |
|---|---|---|
| Configured by | Admin manually | Routing protocols automatically |
| Adapts to failures | No | Yes |
| CPU/bandwidth overhead | None | Some (protocol messages) |
| Scalability | Poor | Good |
| Security | Predictable, no protocol to attack | Needs authentication for protocols |
| Best for | Small/stub networks, default routes, predictable paths | Large, changing networks |

Special static ideas:
- **Default route:** `0.0.0.0/0`.
- **Floating static route:** a backup static route with a high AD that only appears if the dynamic route disappears (e.g., backup via a 4G link).
- **Blackhole/null route:** deliberately drop traffic for a prefix (used in DDoS mitigation).

**Real-world:** Your home router has just one static default route pointing to the ISP. The ISP and the big Internet carriers use dynamic protocols (OSPF/IS-IS inside, BGP between).

---

## 7. Convergence

**Convergence** = the state where all routers have consistent, up-to-date information about the network. After a link fails, the time until everybody agrees on the new paths is the **convergence time**.
- Fast convergence = fewer black holes and loops.
- Link-state protocols usually converge faster than distance-vector protocols.

---

## 8. Classification of Routing Protocols ⭐

```
Routing Protocols
├── By scope
│   ├── IGP (Interior Gateway Protocol): inside one Autonomous System
│   │     RIP, OSPF, EIGRP, IS-IS
│   └── EGP (Exterior Gateway Protocol): between Autonomous Systems
│         BGP
└── By algorithm
    ├── Distance Vector      : RIP, (EIGRP is advanced DV/hybrid)
    ├── Link State           : OSPF, IS-IS
    └── Path Vector          : BGP
```
**Autonomous System (AS):** a collection of networks under one administrative control that presents a common routing policy to the Internet (an ISP, a large company, a cloud provider). Each has a unique **ASN**.

---

## 9. Distance Vector Routing (Bellman-Ford idea) ⭐⭐

### 9.1 Idea
Each router only knows:
- Its **directly connected neighbours**.
- For each destination: the **distance (metric)** and the **vector (direction/next hop)**.

Routers periodically tell their neighbours **"here is my whole routing table"**. Each router improves its own table using its neighbours' info.

**Analogy: rumour-based navigation.** You don't have a map. You ask your neighbour, "How far is Mumbai?" He says "5 stops from me". You add 1 for reaching him, and now you say "Mumbai is 6 stops via him." You just trust what neighbours say ("routing by rumour").

### 9.2 Bellman-Ford equation (concept)
`Distance(me → X) = min over neighbours N of [ cost(me → N) + Distance(N → X) ]`

### 9.3 Process
1. At startup each router knows only its connected networks (distance 0/1).
2. It sends its table to neighbours periodically (e.g., every 30 s in RIP).
3. On receiving a neighbour's table, it adds the link cost, keeps better routes.
4. Over several rounds, information spreads across the network hop by hop.

### 9.4 Weaknesses
- **Slow convergence:** news travels one hop per update cycle.
- **Routing loops** and the **count-to-infinity problem** (bad news travels slowly).
- Sends the **entire table** periodically (bandwidth waste).
- Limited scalability.

### 9.5 Count-to-Infinity problem ⭐⭐

Setup: `A — B — C`, where C is a network attached to B's side. A learns about C via B.
1. Link **B–C fails**. B knows C is unreachable.
2. Before B tells A, **A advertises** "I can reach C in 2 hops" (its old info via B).
3. B thinks, "A reaches C in 2, so I can reach C via A in 3." B updates.
4. B advertises this to A. A updates: "B says 3, so I have 4."
5. They keep bouncing and increasing the metric forever (counting to infinity), while the packets loop between A and B.

**Analogy:** Two friends each think the other one knows the way to a closed shop, and keep telling each other "it's one step more than what you said".

### 9.6 Solutions ⭐

| Technique | Idea |
|---|---|
| **Maximum metric (define infinity)** | RIP treats 16 hops as "unreachable", so counting stops quickly |
| **Split horizon** | Never advertise a route back out the interface you learned it from (A doesn't tell B about C, because it learned it from B) |
| **Route poisoning** | When a route fails, advertise it with metric = infinity (16) immediately |
| **Poison reverse** | With split horizon, explicitly advertise the route back to the source as unreachable (infinity) |
| **Hold-down timers** | After a route is marked down, ignore any updates about it for a while to avoid accepting stale info |
| **Triggered updates** | Send updates immediately when something changes, don't wait for the periodic timer |

Split horizon fixes simple two-router loops but not all larger loops, which is one reason why link-state protocols exist.

---

## 10. Link State Routing (Dijkstra's idea) ⭐⭐

### 10.1 Idea
Every router builds a **complete map** of the network (topology) and independently computes the shortest path to every destination.

**Analogy:** Instead of asking neighbours for opinions (rumours), every router gets a full **Google Maps of the whole city**, then plans its own routes.

### 10.2 Process
1. **Discover neighbours** using Hello packets.
2. **Measure link cost** (usually based on bandwidth).
3. **Create a Link State Advertisement (LSA/LSP)** describing "my links and their costs".
4. **Flood** this advertisement to *all* routers in the area (reliably).
5. Every router stores all advertisements in a **Link State Database (LSDB)**: an identical map on every router.
6. Each router runs **Dijkstra's Shortest Path First (SPF)** algorithm with itself as the root to produce a shortest path tree, from which the routing table is derived.
7. When a link changes, only the **changed info** is flooded (not the entire table), and SPF is re-run.

### 10.3 Advantages
- Fast convergence.
- No count-to-infinity/loops in steady state (everyone has the same map).
- Updates are event-triggered and small.
- Supports hierarchy (areas) for scalability.

### 10.4 Disadvantages
- More CPU and memory (LSDB + SPF).
- More complex to configure.
- Flooding can cause overhead in unstable networks.

---

## 11. Distance Vector vs Link State: Master Comparison ⭐⭐

| Aspect | Distance Vector | Link State |
|---|---|---|
| Knowledge of network | Only neighbours' info ("routing by rumour") | Full topology map |
| Algorithm | Bellman-Ford | Dijkstra (SPF) |
| What is exchanged | Entire routing table | Link state info about own links |
| To whom | Directly connected neighbours only | Flooded to all routers in the area |
| When | Periodic (plus triggered) | Event-triggered (plus periodic refresh) |
| Convergence | Slow | Fast |
| Loop risk | High (count-to-infinity) | Low |
| Resource usage | Low CPU/memory | High CPU/memory |
| Scalability | Small networks | Large networks (with areas) |
| Examples | RIP | OSPF, IS-IS |

---

## 12. Path Vector Routing (BGP concept)

Similar to distance vector, but instead of only a distance, each route advertisement carries the **entire path** (list of Autonomous Systems it traverses).
- A router can detect a **loop** just by seeing its own AS number in the path (**AS-PATH loop prevention**).
- Allows policy-based decisions ("don't use AS X").
- Used by BGP (deep dive in file 2).

---

## 13. Routing Loops and How They Are Prevented ⭐

A **routing loop** is when packets keep circling among routers because their tables disagree.

Causes: slow convergence after a failure, misconfiguration (bad static routes), route redistribution errors.

Prevention/mitigation:
- **TTL** in the IP header (packet is dropped after N hops; universal safety net). ⭐
- Split horizon, poison reverse, hold-down (distance vector).
- Link-state's consistent map.
- Loop-free design; AS-PATH check in BGP.
- Note: Layer 2 has no TTL, hence STP is needed there.

---

## 14. Communication Types (Routing perspective) ⭐

| Type | Meaning | Example |
|---|---|---|
| **Unicast** | One sender → one receiver | Loading a webpage |
| **Broadcast** | One → all in the local network (not routed across routers) | ARP request, DHCP discover |
| **Multicast** | One → a subscribed group (`224.0.0.0/4`, IPv6 `ff00::/8`) | IPTV, stock ticker feeds, OSPF Hello (224.0.0.5) |
| **Anycast** | One → the *nearest* of several nodes sharing the same IP | Google DNS 8.8.8.8, Cloudflare 1.1.1.1, CDNs |

### Anycast in detail (system design favourite) ⭐
Multiple servers around the world announce **the same IP** through BGP. Routing naturally sends each user to the topologically closest one.
- Benefits: low latency, automatic failover (if one site dies, its announcement disappears and traffic flows to the next nearest), DDoS absorption across many sites.
- Used by: DNS root servers, public DNS resolvers, CDNs (Cloudflare, Akamai), DDoS scrubbing.
- Caveat: with TCP, if routing shifts mid-connection, packets can hit a different server; works best for short/stateless flows (DNS) or with careful design.

---

## 15. Other Routing Concepts Worth Knowing

### 15.1 ECMP (Equal-Cost Multi-Path)
If several paths have the same cost, the router load-balances flows across them (hash on source/destination IP and ports so a single connection stays on one path). Widely used in data centers (Clos / leaf-spine fabrics).

### 15.2 Policy-Based Routing (PBR)
Route based on something other than destination (source address, application, port). Example: send video conferencing over the low-latency link and backups over the cheap link.

### 15.3 Route Redistribution
Injecting routes learned from one protocol into another (e.g., OSPF ↔ BGP). Powerful but dangerous if done carelessly (loops, suboptimal paths).

### 15.4 Route Summarization / Aggregation
Combine many specific prefixes into a single shorter prefix to shrink routing tables and hide instability.
`10.1.0.0/24, 10.1.1.0/24, 10.1.2.0/24, 10.1.3.0/24` → `10.1.0.0/22`

### 15.5 Hierarchical routing
The Internet is too big for a flat scheme. Split into Autonomous Systems (inside: IGP; between: BGP), and inside large ASes further into areas (OSPF).

### 15.6 VRF (Virtual Routing and Forwarding)
Multiple isolated routing tables on the same router (multi-tenant ISPs, MPLS VPNs).

### 15.7 Host-level routing (relevant to DevOps)
Your laptop, server or container also has a routing table. Linux `ip route`, Docker bridge, Kubernetes CNI plugins (Calico uses BGP to distribute pod routes; Flannel uses overlays) rely on the same ideas.

---

## 16. Troubleshooting Routing

| Symptom | Likely cause | Command |
|---|---|---|
| Can ping local hosts but not the Internet | Missing/wrong default gateway | `ip route`, `route print` |
| Traffic goes the wrong way | Wrong/more specific route | `ip route get <ip>` |
| Path looks strange | Asymmetric or suboptimal route | `traceroute`, `mtr` |
| Packets loop | Routing loop | traceroute shows repeating hops |
| Intermittent connectivity | Flapping link/route | routing logs, `show ip route` (on routers) |

Note: Internet routing is often **asymmetric**: forward and return paths can differ. Traceroute shows only the forward path.

---

## 17. Interview Q&A

**Q1. What is the difference between routing and forwarding?** ⭐
Routing builds the routing table (control plane, protocols). Forwarding uses it per packet to send it out of an interface (data plane, hardware fast path).

**Q2. What is a routing table? What does it contain?**
The router's list of known destinations with next hop, interface, metric and route source. See Section 3.

**Q3. What is longest prefix match?** ⭐⭐
When multiple routes match, the most specific (longest prefix) wins. Give the 10.0.0.0/8, /16, /24 example.

**Q4. Static vs dynamic routing?** ⭐ Section 6.

**Q5. What is a default route?**
`0.0.0.0/0`, the route of last resort, used when no more specific route matches.

**Q6. Distance Vector vs Link State?** ⭐⭐ Section 11 table + rumour vs map analogy.

**Q7. What is count-to-infinity? How do you solve it?** ⭐⭐
Slow propagation of a link failure in DV protocols where routers keep increasing the metric while learning from each other. Solved using max hop limit (16 in RIP), split horizon, poison reverse, hold-down, triggered updates.

**Q8. What is split horizon vs poison reverse?**
Split horizon: don't advertise a route back on the interface you learned it from. Poison reverse: advertise it back but with infinite metric, making it explicit.

**Q9. What is metric vs administrative distance?** ⭐
Metric compares routes within the same protocol. AD compares trustworthiness across different protocols/sources. Longest prefix match happens first, then AD, then metric.

**Q10. What is convergence?**
The time and state when all routers agree on a consistent view of the topology after a change.

**Q11. What is an Autonomous System?**
A network (or group) under a single administrative entity with a common routing policy, identified by an ASN, e.g., Jio, Airtel, Google, AWS.

**Q12. IGP vs EGP?** ⭐
IGP routes within an AS (OSPF, RIP, IS-IS, EIGRP). EGP routes between ASes (BGP).

**Q13. How does a router prevent routing loops?**
TTL, protocol-level techniques (split horizon etc.), consistent link-state maps, AS-PATH check in BGP.

**Q14. What is anycast and where is it used?** ⭐
Same IP announced from many locations; users reach the nearest. DNS resolvers, CDNs, DDoS mitigation.

**Q15. What is ECMP?**
Load balancing across multiple equal-cost paths, using a flow hash to keep packets of one connection in order. Common in data centers.

**Q16. What happens at each router hop to a packet?**
Strip L2 frame → read destination IP → longest prefix match → TTL−1 (and checksum update) → find next hop, ARP → new L2 frame → send. The IP addresses remain unchanged (except NAT); MAC addresses change.

---

## 18. Cheat Sheet

- Routing = build map (control plane). Forwarding = move packet (data plane).
- Decision order: **Longest prefix → lowest AD → lowest metric**.
- AD: Connected 0, Static 1, eBGP 20, OSPF 110, RIP 120, iBGP 200.
- DV = Bellman-Ford, rumour, periodic full table, slow, count-to-infinity. Example: RIP.
- LS = Dijkstra, full map, flooded LSAs, fast. Examples: OSPF, IS-IS.
- Path vector = BGP, AS-PATH prevents loops.
- Fixes for DV problems: max metric (16), split horizon, poison reverse, hold-down, triggered updates.
- TTL protects against forever loops.
- Anycast = same IP in many places, nearest wins.
