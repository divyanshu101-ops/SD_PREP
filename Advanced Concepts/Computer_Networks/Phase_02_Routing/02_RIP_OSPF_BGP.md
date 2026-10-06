# Phase 2 · File 2: RIP, OSPF, EIGRP, BGP and How the Internet is Structured

> **Goal:** understand the real routing protocols, when each is used, how BGP glues the Internet together, and why BGP mistakes can take down half the world.

**Analogy:** Think of countries and cities.
- **Inside a city (an Autonomous System)** the municipal transport authority plans routes with full maps → **OSPF / IS-IS** (link state).
- **Between countries (between ASes)** governments negotiate agreements ("we accept your trucks, not theirs") → **BGP**, which is driven by *politics and business policy*, not just shortest distance.

---

## 1. RIP: Routing Information Protocol

The oldest widely-known IGP. Simple, easy to configure, limited.

| Property | Value |
|---|---|
| Type | Distance Vector |
| Metric | **Hop count** |
| Max hops | **15** (16 = unreachable) |
| Update | Full table every **30 seconds** |
| Transport | UDP port **520** |
| AD | 120 |
| Versions | RIPv1 (classful, broadcast, no auth), **RIPv2** (classless/CIDR, multicast 224.0.0.9, authentication), **RIPng** (IPv6) |

### Timers (good to recognise)
- Update: 30 s
- Invalid: 180 s (route considered invalid if no update)
- Hold-down: 180 s
- Flush: 240 s (route removed)

### Why RIP is rarely used today
- Hop count ignores link speed: 2 hops over slow 1 Mbps links beats 3 hops over 10 Gbps.
- 15-hop limit stops it scaling.
- Slow convergence, periodic full-table updates waste bandwidth.

**Still useful for:** learning, tiny/simple networks, lab exams.

---

## 2. OSPF: Open Shortest Path First ⭐⭐

The most common IGP in enterprises and ISPs.

| Property | Value |
|---|---|
| Type | **Link State** (Dijkstra SPF) |
| Metric | **Cost** (inversely related to link bandwidth: faster link = lower cost) |
| Transport | Runs directly over IP (**protocol number 89**), not TCP/UDP |
| Multicast addresses | 224.0.0.5 (all OSPF routers), 224.0.0.6 (DR/BDR) |
| AD | 110 |
| Standard | Open standard (not vendor-specific), OSPFv2 for IPv4, OSPFv3 for IPv6 |
| Supports | VLSM, CIDR, authentication, equal-cost multipath |

### 2.1 Key concepts

**Router ID (RID):** unique 32-bit identifier of each OSPF router (often the highest loopback IP).

**Areas (hierarchy for scalability):**
- A large OSPF network is split into **areas** to limit the size of the LSDB and the SPF calculation scope.
- **Area 0 = backbone area.** All other areas must connect to Area 0 (directly or via virtual link).
- **ABR (Area Border Router):** connects an area to Area 0.
- **ASBR (Autonomous System Boundary Router):** injects routes from outside OSPF (e.g., from BGP/static).
- Internal routers are inside one area only.

```
        Area 1            Area 0 (backbone)          Area 2
     [R1]--[R2]==ABR==[R3]--[R4]--[R5]==ABR==[R6]--[R7]
                                      |
                                    ASBR --- to Internet (BGP)
```

**Adjacency and neighbours:** routers become neighbours after exchanging Hello packets, then form **adjacencies** and synchronise databases.

### 2.2 OSPF operation (step by step) ⭐
1. **Hello packets** (default every 10 s on broadcast networks, dead interval 40 s) discover neighbours and check that parameters (area ID, timers, authentication, subnet) match.
2. **Neighbour states** progress: Down → Init → 2-Way → ExStart → Exchange → Loading → **Full** (you don't need to memorise each, but know that "Full" means databases are synchronised).
3. Routers exchange **LSAs (Link State Advertisements)** describing links.
4. Every router in the area builds an identical **LSDB**.
5. Each runs **SPF (Dijkstra)** to compute best paths and fills the routing table.
6. On any topology change, a new LSA is flooded and SPF is re-run.

### 2.3 DR and BDR (Designated Router / Backup DR) ⭐
On a shared multi-access segment (like an Ethernet LAN with many routers), if every router formed an adjacency with every other, we'd get a mesh of n(n−1)/2 adjacencies → too much traffic.
- One router is elected **DR**; another is the **BDR** (backup).
- All other routers form adjacencies only with the DR/BDR and send updates to them (224.0.0.6). The DR floods them out to everyone (224.0.0.5).
- Election: highest OSPF priority, then highest router ID. Not pre-emptive by default.

**Analogy:** In a class, instead of everyone telling everyone else the announcement, students tell the **class representative** who tells all others.

### 2.4 Cost
`Cost = Reference bandwidth / Interface bandwidth`. The path with the lowest total cost wins. (In modern networks the reference bandwidth is tuned so 10 Gbps/100 Gbps links are distinguishable.)

### 2.5 LSA types (just recognise)
Type 1 Router LSA, Type 2 Network LSA (by DR), Type 3 Summary LSA (between areas, by ABR), Type 4 ASBR summary, Type 5 External LSA (routes from outside). Also special area types: **Stub, Totally Stubby, NSSA** reduce LSAs by replacing external routes with a default route.

### 2.6 OSPF advantages/disadvantages
✅ Fast convergence, scalable with areas, loop-free, vendor-neutral, supports large enterprises.
❌ More complex to design/configure, needs more memory/CPU than RIP, area design matters.

---

## 3. IS-IS and EIGRP (know the names and one-liners)

### IS-IS (Intermediate System to Intermediate System)
- Link-state like OSPF, runs directly on Layer 2 (not IP), uses levels (L1 intra-area, L2 backbone).
- Very popular in **large ISP and data-center backbones**; very stable and scalable.

### EIGRP (Enhanced Interior Gateway Routing Protocol)
- Cisco-developed (later opened partially), "advanced distance vector" / hybrid.
- Uses **DUAL** algorithm for loop-free fast convergence; metric based on bandwidth + delay; AD 90.
- Keeps **successor** (best route) and **feasible successor** (pre-computed backup) for instant failover.

---

## 4. Autonomous Systems and the Structure of the Internet ⭐

The Internet is a **network of ~100,000+ Autonomous Systems** that voluntarily interconnect.

### 4.1 AS and ASN
- **AS** = group of IP prefixes under one administrative control with a single routing policy.
- **ASN** = its identifier (16-bit originally, now 32-bit). Examples: Google AS15169, Cloudflare AS13335, Amazon AS16509, Reliance Jio AS55836, Bharti Airtel AS9498.
- ASNs and IP blocks are allocated by IANA → **RIRs** (ARIN, RIPE, **APNIC**, LACNIC, AFRINIC). India is under APNIC; IN's national exchange body is NIXI.

### 4.2 ISP tiers ⭐

| Tier | Description | Examples |
|---|---|---|
| **Tier 1** | Global backbone networks that can reach the entire Internet **without paying anyone** (they only peer with each other) | Lumen, NTT, Telia, Tata Communications, Arelion |
| **Tier 2** | Regional ISPs; buy transit from Tier 1 and peer with others | Many national ISPs |
| **Tier 3** | Access/last-mile ISPs selling to homes and businesses; buy transit | Local broadband providers |

Content providers (Google, Netflix, Meta, Cloudflare) run their own huge networks and connect directly to ISPs, bypassing much of the tier hierarchy.

### 4.3 Peering vs Transit ⭐

| | Transit | Peering |
|---|---|---|
| Relationship | Customer **pays** provider to reach the whole Internet | Two networks exchange traffic **for each other's customers** only, usually free (settlement-free) |
| Reach | Full Internet | Only the peer's own customers |
| Money | Customer → provider | Usually none |

**Analogy:** Transit = paying a big courier company to deliver anywhere in the world. Peering = two neighbouring companies agreeing to deliver each other's parcels within their own areas for free.

### 4.4 IXP: Internet Exchange Point
A physical location where many networks connect to peer with each other via a shared switch fabric. Reduces cost and latency (traffic between two Indian ISPs stays in India instead of going abroad).
Examples: NIXI (India), DE-CIX, AMS-IX, LINX.

### 4.5 CDN and content edge
Companies place cache servers inside ISP networks and at IXPs (Netflix Open Connect, Google Global Cache, Akamai, Cloudflare) so that popular content is served from nearby.

---

## 5. BGP: Border Gateway Protocol ⭐⭐⭐

**The routing protocol of the Internet.** BGP-4 is the only EGP used today.

| Property | Value |
|---|---|
| Type | **Path Vector** |
| Transport | **TCP port 179** (reliable sessions between configured peers) |
| Scope | Between Autonomous Systems (eBGP) and within one AS (iBGP) |
| AD | eBGP 20, iBGP 200 |
| Decision basis | **Policy and attributes**, not just shortest distance |
| Updates | Incremental (only changes) after initial full exchange |

### 5.1 Why BGP is different from OSPF
- OSPF asks: "What is the *shortest/cheapest* path?"
- BGP asks: "What is the *best path according to business policy*?" (cheapest for us financially, preferred customer, avoid competitor).
- Internet routing is about **contracts and trust**, so BGP gives operators fine control.

### 5.2 eBGP vs iBGP ⭐

| | eBGP | iBGP |
|---|---|---|
| Between | Different ASes | Routers inside the same AS |
| Purpose | Exchange routes with neighbouring networks | Distribute externally learned routes internally |
| AD | 20 | 200 |
| TTL | Peers usually directly connected | Can be multi-hop |
| Loop prevention | AS-PATH | iBGP routes not re-advertised to other iBGP peers (split horizon rule) → needs full mesh, route reflectors or confederations |

### 5.3 How BGP works (step by step)
1. **TCP session** established on port 179 between two configured peers.
2. **OPEN** message: exchange ASN, hold time, router ID, capabilities.
3. **UPDATE** messages: advertise reachable prefixes with **path attributes**, or withdraw prefixes.
4. **KEEPALIVE** messages maintain the session; **NOTIFICATION** reports errors and closes it.
5. Each BGP router runs the **best-path selection** and puts only the best route in the routing table. It then advertises that best route (to allowed peers, according to policy).

### 5.4 Important path attributes ⭐
- **AS-PATH:** list of ASNs the route has traversed. Shorter is preferred; used for **loop prevention** (reject any route that contains my own ASN). Prepending your own ASN multiple times makes a path look longer (traffic engineering).
- **NEXT-HOP:** IP address to forward to.
- **LOCAL_PREF:** *inside your AS* preference (higher = better). Used to choose which exit you like the most (e.g., prefer the cheap ISP).
- **MED (Multi-Exit Discriminator):** hint *to a neighbour AS* about which of your entry points you prefer (lower = better).
- **Origin, Weight (Cisco-local), Communities (tags to signal policy, e.g., "don't export", "blackhole"), Atomic aggregate/aggregator.**

### 5.5 Best-path selection (simplified order) ⭐
1. Highest Weight (Cisco local).
2. **Highest LOCAL_PREF.**
3. Locally originated routes preferred.
4. **Shortest AS-PATH.**
5. Lowest Origin type.
6. **Lowest MED.**
7. eBGP over iBGP.
8. Lowest IGP metric to next hop.
9. Tie-breakers (oldest route, lowest router ID).

Memory trick: "**W**e **L**ove **O**ranges **A**s **O**ranges **M**ean **N**ice **I**ce-cream" (Weight, Local pref, Originate, AS-path, Origin, MED, Neighbour type, IGP metric). Just know local-pref → AS-path → MED conceptually.

### 5.6 Route policy: the real power
- **Prefix filters / prefix lists:** accept/announce only expected prefixes.
- **Route maps:** modify attributes.
- **Business relationships** implemented via policy (Gao-Rexford model):
  - Prefer routes from **customers** (they pay you) > **peers** (free) > **providers** (you pay).
  - Advertise customer routes to everyone; advertise peer/provider routes only to customers. (Otherwise you'd carry free transit for others.)

### 5.7 BGP scalability: iBGP solutions
- **Route reflectors:** a designated router "reflects" iBGP routes to clients so a full mesh isn't needed.
- **Confederations:** split an AS into sub-ASes.

### 5.8 Convergence and stability problems
- BGP converges **slowly** (minutes) after big changes, deliberately, to be stable at global scale.
- **Route flapping:** a prefix repeatedly appears/disappears → heavy churn. Mitigated by **route flap dampening**.
- Global routing table size (~1 million IPv4 prefixes) requires powerful routers; aggregation helps.

---

## 6. BGP Security: Hijacks and Leaks ⭐⭐

BGP was designed on **trust**: originally, any AS could announce any prefix and neighbours would believe it.

### 6.1 Prefix hijacking
An AS (by mistake or maliciously) announces IP prefixes it doesn't own. Because of **longest prefix match**, announcing a *more specific* prefix attracts traffic.

### 6.2 Route leak
An AS re-advertises routes to peers/providers that it should not (e.g., a small customer advertises full Internet routes to another provider), attracting traffic it can't handle.

### 6.3 Famous incidents (real-world)
| Year | Event | Lesson |
|---|---|---|
| 2008 | **Pakistan Telecom vs YouTube:** Pakistan tried to block YouTube domestically by announcing a more specific YouTube prefix; it leaked to the world, and YouTube went dark globally for about two hours. | More specific prefix wins; filter what customers announce. |
| 2018 | **Amazon Route 53 / MyEtherWallet hijack:** attackers announced Amazon DNS prefixes, redirecting users of a crypto wallet to a fake site. | BGP + DNS attack combined to steal money. |
| 2019 | A small ISP leaked routes and caused a major outage for Cloudflare and other services. | Route leaks, lack of filtering. |
| 2021 | **Facebook outage (Oct 2021):** a configuration change withdrew BGP routes to Facebook's DNS servers; Facebook, Instagram and WhatsApp disappeared for about 6 hours. Internal tools also failed since they relied on the same network. | Out-of-band access and dependency planning matter. |

### 6.4 Defences
- **Prefix and AS-PATH filtering**, max-prefix limits.
- **RPKI (Resource Public Key Infrastructure):** cryptographic **ROAs** (Route Origin Authorizations) state which AS is allowed to originate a prefix; routers perform **Route Origin Validation**.
- **BGPsec** (full path validation, not widely deployed), **MANRS** norms (routing security best practices), monitoring (BGPStream, RIPE RIS).

---

## 7. MPLS and SDN (know at a high level)

### MPLS: Multiprotocol Label Switching
- Packets get a short **label** at the ingress router; core routers forward based on labels (fast lookup, no full IP lookups), the label is removed at egress.
- Enables traffic engineering, fast reroute, and **VPNs** for enterprises (MPLS L3VPN). Sits between L2 and L3 ("Layer 2.5").
- Being partially replaced by SD-WAN and Segment Routing in newer designs.

### SDN: Software Defined Networking
- Separates control plane (central controller) from data plane (simple switches), programmable through APIs (OpenFlow).
- Used in cloud and data-center networks (Google B4, AWS/Azure virtual networking). This is the "infrastructure as code" mindset applied to networking.

### SD-WAN
Manages multiple WAN links (MPLS, broadband, 4G) centrally and steers traffic based on application needs; replaces expensive private lines for branch offices.

---

## 8. Data-Center and Cloud Routing (relevant to DevOps)

- **Leaf-spine (Clos) fabric:** every leaf switch connects to every spine switch; any two servers are the same number of hops apart. Uses **ECMP** for load balancing. Often runs **BGP** even inside the data center (simple, scalable).
- **Overlay networks (VXLAN):** encapsulate tenant traffic inside UDP to create virtual L2 networks across L3 fabric. Used by cloud providers and Kubernetes CNIs.
- **AWS VPC routing:** route tables attached to subnets; entries like `10.0.0.0/16 → local`, `0.0.0.0/0 → igw-xxxx` (public subnet) or `0.0.0.0/0 → nat-xxxx` (private subnet). Longest prefix match applies here too.
- **Kubernetes:** Calico can peer with your network using BGP to advertise pod CIDRs; kube-proxy/iptables handle Service virtual IPs.
- **BGP with cloud direct connect / VPN:** on-prem to cloud links exchange routes via BGP.

---

## 9. Case Study: "How does a request from Kolkata reach google.com?" ⭐⭐

1. Your phone gets a private/CGNAT IP from your ISP (e.g., via DHCP). The default gateway is the ISP's access router.
2. DNS resolves `google.com` to an IP (say 142.250.x.x) that may be **anycasted** or belong to a nearby Google edge.
3. Packet goes to your home/ISP router (default route). ISP's **IGP (OSPF/IS-IS)** carries it to the ISP border router.
4. The ISP's border router consults its **BGP table**: Google (AS15169) is reachable via a **direct peering** at an IXP or Google Global Cache node inside the ISP (short AS-PATH, preferred by LOCAL_PREF because peers are cheaper than transit).
5. Packet crosses the peering link into Google's AS, whose internal network (with its own IGP and SDN) forwards it to the correct edge/front-end server.
6. TCP/TLS handshakes happen; response follows a return path which may differ (asymmetric routing).
7. If Google cache isn't inside the ISP, traffic might go via a Tier 1 like Tata Communications or across undersea cables. Traceroute reveals some of these hops.

Each hop: longest prefix match → next hop → new L2 frame; IP source/destination unchanged (except CGNAT changes source).

---

## 10. Protocol Comparison Table ⭐

| | RIP | OSPF | EIGRP | IS-IS | BGP |
|---|---|---|---|---|---|
| Category | IGP | IGP | IGP | IGP | EGP |
| Algorithm | Distance vector | Link state | Advanced DV (DUAL) | Link state | Path vector |
| Metric | Hop count | Cost (bandwidth) | Bandwidth + delay | Cost | Attributes/policy |
| Transport | UDP 520 | IP proto 89 | IP proto 88 | Layer 2 (CLNS) | TCP 179 |
| AD | 120 | 110 | 90 | 115 | eBGP 20 / iBGP 200 |
| Convergence | Slow | Fast | Very fast | Fast | Slow (by design) |
| Scale | Small | Large (with areas) | Medium-large | Very large | Internet |
| Typical use | Labs, tiny networks | Enterprise/ISP core | Cisco shops | ISP/data center backbones | Between ISPs, cloud, data centers |

---

## 11. Interview Q&A

**Q1. RIP vs OSPF?** ⭐
RIP is distance vector using hop count, max 15 hops, periodic full-table updates, slow convergence. OSPF is link-state with cost metric, areas, fast convergence, scales to large networks.

**Q2. Explain OSPF in simple words.** ⭐⭐
Every router describes its links, floods that to all routers in the area, so everyone has the same map; each runs Dijkstra to find shortest paths. Areas (with Area 0 backbone) keep it scalable.

**Q3. What are OSPF areas and why?**
Divide a large network to limit LSA flooding and SPF calculation size, improve stability, and enable summarization. Area 0 is the backbone.

**Q4. What is DR/BDR?**
Elected routers on multi-access networks to reduce adjacency overhead; others sync only with the DR/BDR.

**Q5. What is BGP and why is it needed?** ⭐⭐
The Internet's inter-domain routing protocol. IGPs can't scale or express business policy across independent organisations. BGP exchanges reachability between Autonomous Systems using paths and policies.

**Q6. OSPF vs BGP: when do you use which?** ⭐⭐
OSPF inside one organisation to find the fastest internal paths. BGP between organisations (ISPs, cloud, enterprise-to-ISP) to enforce policy. Often used together: BGP carries external routes, OSPF/IS-IS reaches the BGP next hops.

**Q7. eBGP vs iBGP?**
eBGP is between different ASes (AD 20); iBGP is inside an AS to spread external routes (AD 200) and requires full mesh or route reflectors due to its no-re-advertise rule.

**Q8. Why does BGP use TCP?**
It needs reliable, ordered delivery of large updates and session state; TCP handles retransmission, so BGP doesn't implement it.

**Q9. How does BGP choose the best path?** ⭐
Local preference → shortest AS-PATH → origin → lowest MED → eBGP over iBGP → lowest IGP cost → tie-breakers. (Weight is Cisco-only and first.)

**Q10. What is an ASN? Give examples.**
Unique number for an Autonomous System: Google 15169, Cloudflare 13335, AWS 16509.

**Q11. What is BGP hijacking, how can it be prevented?** ⭐⭐
False origin announcement or more specific prefix attracts traffic. Mitigate with prefix filtering, max-prefix limits, RPKI ROAs with route origin validation, monitoring. Mention Pakistan/YouTube 2008.

**Q12. What caused the 2021 Facebook outage?**
A configuration change withdrew BGP routes to their DNS servers, so nobody could resolve or reach Facebook; internal tooling also depended on the same network, delaying recovery.

**Q13. Peering vs transit?** ⭐ Section 4.3.

**Q14. What is an IXP and why does it matter?**
A shared exchange where networks interconnect directly; reduces latency and transit costs, keeps local traffic local.

**Q15. What is a Tier 1 ISP?**
A network that reaches the entire Internet purely via settlement-free peering with other Tier 1s, without buying transit.

**Q16. What is MPLS?**
Label-based forwarding through a provider core; enables traffic engineering and VPNs.

**Q17. How does routing work in AWS VPC?**
Route tables per subnet; longest prefix match; `local` route for VPC CIDR; `0.0.0.0/0` to an Internet Gateway (public) or NAT Gateway (private); more specific routes via peering, VPN, Transit Gateway.

**Q18. Why is BGP convergence slow?**
It is deliberately conservative (timers, path exploration, dampening) to remain stable across a massive, independently-run network.

**Q19. What is route aggregation and why is it important?**
Combining prefixes into a shorter one to reduce routing table size and hide internal instability. Essential for Internet scalability.

---

## 12. Cheat Sheet

- **RIP:** DV, hop count, max 15, UDP 520, AD 120, 30 s updates.
- **OSPF:** link-state, cost, IP proto 89, AD 110, Area 0 backbone, Hello 10 s / Dead 40 s, DR/BDR on LANs.
- **EIGRP:** Cisco, DUAL, AD 90. **IS-IS:** link-state, ISP favourite.
- **BGP:** path vector, TCP 179, AS-PATH, LOCAL_PREF (inside AS), MED (to neighbours), eBGP AD 20 / iBGP AD 200.
- Internet = ~100k+ ASes; Tier 1/2/3; peering (free) vs transit (paid); IXPs.
- BGP risks: hijack, leak. Fix: filtering + RPKI. Remember Pakistan/YouTube 2008 and Facebook 2021.
- Cloud: VPC route tables, IGW vs NAT GW, BGP for Direct Connect/VPN, Calico BGP, VXLAN overlays.
- IGP = inside AS (find shortest path). EGP/BGP = between AS (apply policy).
