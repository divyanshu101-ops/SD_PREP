# Phase 1 · File 3: Network Layer: IP, Subnetting, NAT, DHCP, ICMP

> **Goal:** understand IP addressing (IPv4 and IPv6), how networks are divided (subnetting/CIDR), how private networks reach the Internet (NAT), how devices get addresses (DHCP), and how `ping`/`traceroute` actually work (ICMP).

**Analogy:** If the data link layer is "inside one building", the network layer is the **postal system between buildings and cities**. IP address = full postal address. The network layer's job is to move a packet from *any host to any other host* on the planet, hop by hop, through routers.

---

## 1. What the Network Layer Does

1. **Logical addressing:** every host gets an IP address.
2. **Routing/forwarding:** routers decide the next hop for each packet (deep dive in Phase 2).
3. **Fragmentation and reassembly** when packets exceed a link's MTU.
4. **Best-effort delivery:** IP does **not** guarantee delivery, order, or no-duplication. Reliability is TCP's job. ⭐

**Analogy:** IP is like dropping letters in the postbox: the postal service tries its best, but doesn't promise you a receipt. TCP adds tracking and acknowledgements on top.

Key protocols: **IP (v4, v6), ICMP, ARP/NDP (helpers), IPsec, routing protocols (OSPF, BGP)**.

---

## 2. IPv4 Address Basics ⭐

- **32 bits**, written as four decimal numbers (octets) separated by dots: `192.168.1.10`.
- Total about 4.3 billion addresses (2^32). Now exhausted → NAT and IPv6.
- Every address has two parts: **Network part** (which network) and **Host part** (which device inside it). The **subnet mask** or **prefix length** tells where the split is.

```
IP address :  192 . 168 . 1 . 10
Binary     :  11000000.10101000.00000001.00001010
Mask /24   :  11111111.11111111.11111111.00000000  (255.255.255.0)
              |------ network ------|--- host ---|
```

### 2.1 Classful addressing (historical, know for theory)

| Class | First octet | Default mask | Use |
|---|---|---|---|
| A | 1-126 | /8 | Very large networks |
| B | 128-191 | /16 | Medium networks |
| C | 192-223 | /24 | Small networks |
| D | 224-239 | n/a | **Multicast** |
| E | 240-255 | n/a | Experimental |

Modern networks use **classless (CIDR)** addressing, but classes still show up in interviews.

### 2.2 Public vs Private IP ⭐

- **Public IP:** globally unique, routable on the Internet, assigned by ISPs (originally by IANA → RIRs like APNIC for Asia-Pacific).
- **Private IP:** reusable inside any local network, **not routable on the Internet** (RFC 1918):

| Range | CIDR | Typical use |
|---|---|---|
| 10.0.0.0 - 10.255.255.255 | 10.0.0.0/8 | Large enterprises, cloud VPCs |
| 172.16.0.0 - 172.31.255.255 | 172.16.0.0/12 | Medium networks, Docker default |
| 192.168.0.0 - 192.168.255.255 | 192.168.0.0/16 | Home routers |

**Analogy:** Private IP = **flat number inside your apartment complex** (many complexes can have "Flat 101"). Public IP = the **complex's street address** that the postman uses.

### 2.3 Special addresses ⭐

| Address | Meaning |
|---|---|
| `127.0.0.0/8` (e.g., 127.0.0.1) | **Loopback**: "this machine" (localhost) |
| `0.0.0.0` | "Any / unspecified" (a server listening on 0.0.0.0 accepts connections on all interfaces) |
| `255.255.255.255` | Limited broadcast |
| `x.x.x.255` (host bits all 1) | Directed broadcast for that subnet |
| `x.x.x.0` (host bits all 0) | Network address |
| `169.254.0.0/16` | **APIPA / link-local**: self-assigned when DHCP fails |
| `100.64.0.0/10` | **CGNAT** shared address space used by ISPs |
| `224.0.0.0/4` | Multicast |

**Usable hosts in a subnet = 2^(host bits) − 2** (minus network and broadcast address). You don't need to calculate numbers in an interview, but you should know *why* two addresses are lost.

---

## 3. Subnetting and CIDR ⭐ (concept level)

### 3.1 Why subnet?
A single huge network is inefficient: too many broadcasts, hard to secure, wasteful. **Subnetting** divides one network into smaller ones.

**Analogy:** A big housing society divided into blocks A, B, C. Each block has its own security gate and notice board (broadcast domain).

Benefits: less broadcast traffic, better security (firewall between subnets), organized addressing (one subnet per department/floor/region), efficient use of addresses.

### 3.2 CIDR: Classless Inter-Domain Routing
Instead of fixed classes, we write `address/prefix-length`:
- `192.168.1.0/24` → first 24 bits are network, last 8 bits host.
- `10.0.0.0/16` → 16 network bits, 16 host bits.
- **Bigger prefix number = smaller network** (`/28` is smaller than `/24`).

CIDR also enables **route aggregation (supernetting)**: many small networks advertised as one bigger prefix, keeping Internet routing tables small.

### 3.3 VLSM: Variable Length Subnet Mask
Different subnets can have different sizes according to need: a point-to-point router link needs only 2 hosts (`/30` or `/31`), a server network may need 200 (`/24`).

### 3.4 Quick conceptual example (no heavy math)
Company has `192.168.10.0/24` and wants 4 departments. Borrow 2 host bits for subnetting → each subnet is `/26`:
- 192.168.10.0/26 (Dept 1)
- 192.168.10.64/26 (Dept 2)
- 192.168.10.128/26 (Dept 3)
- 192.168.10.192/26 (Dept 4)

Each has 64 addresses (62 usable). Boundaries jump by 64. That's all the calculation typically expected in a concept-based interview.

### 3.5 Cloud relevance
When you create an AWS **VPC** with `10.0.0.0/16`, you split it into **subnets** (e.g., `10.0.1.0/24` public, `10.0.2.0/24` private). Kubernetes pod networks, Docker bridge networks use the same ideas. So subnetting is not just theory.

---

## 4. IPv4 Header ⭐

```
 0       4       8              16      19                  31
+-------+-------+---------------+--------+-------------------+
|Version| IHL   |   DSCP/ECN    |        Total Length        |
+-------+-------+---------------+--------+-------------------+
|        Identification         | Flags  |  Fragment Offset  |
+-------------------------------+--------+-------------------+
|     TTL       |   Protocol    |       Header Checksum      |
+---------------+---------------+----------------------------+
|                     Source IP Address                      |
+------------------------------------------------------------+
|                  Destination IP Address                    |
+------------------------------------------------------------+
|                Options (rare) + Padding                    |
+------------------------------------------------------------+
```

| Field | Purpose |
|---|---|
| **Version** | 4 |
| **IHL** | Header length (min 20 bytes) |
| **DSCP/ECN** | QoS priority, congestion notification |
| **Total Length** | Packet size (header + data) |
| **Identification, Flags, Fragment Offset** | Used for fragmentation/reassembly |
| **TTL (Time To Live)** | Max hops; decremented by each router; at 0 the packet is dropped and ICMP "Time Exceeded" is sent. **Prevents infinite routing loops.** ⭐ |
| **Protocol** | Which L4 protocol is inside: 6=TCP, 17=UDP, 1=ICMP, 89=OSPF |
| **Header Checksum** | Detects header corruption (recalculated at each hop as TTL changes) |
| **Source / Destination IP** | Addresses |

### Fragmentation ⭐
Each link has an MTU (Ethernet 1500 bytes). If an IP packet is bigger, IPv4 routers may **fragment** it into smaller pieces; the **destination** reassembles them (routers don't reassemble). Problems: extra overhead, one lost fragment loses the whole packet.

- **DF (Don't Fragment) flag:** if set and the packet is too big, the router drops it and sends ICMP "Fragmentation needed".
- **Path MTU Discovery (PMTUD):** sender sets DF and learns the smallest MTU along the path using those ICMP messages. Blocking ICMP in firewalls breaks PMTUD and causes mysterious hangs (a classic real-world bug!).
- IPv6 routers **never fragment**; only the sender may.

---

## 5. IPv6 ⭐

**Why?** IPv4 addresses ran out. IPv6 has **128-bit** addresses (~3.4 × 10^38), enough for every grain of sand.

Format: 8 groups of 4 hex digits separated by colons:
`2001:0db8:0000:0000:0000:ff00:0042:8329`
Shortening rules: drop leading zeros in each group, and replace **one** run of all-zero groups with `::` → `2001:db8::ff00:42:8329`.

### Key features
- **Simpler fixed 40-byte header**, no header checksum, no router fragmentation → faster routing.
- **No broadcast.** Uses multicast and anycast instead.
- **Built-in autoconfiguration:** **SLAAC** (Stateless Address Autoconfiguration): a host builds its own address from the network prefix + interface ID. DHCPv6 is also available.
- **NDP (Neighbor Discovery Protocol)** replaces ARP (uses ICMPv6).
- **IPsec** support is part of the design.
- **No NAT needed** (every device can have a globally unique address; firewalls provide protection).

### Address types
| Type | Example prefix | Meaning |
|---|---|---|
| Global unicast | `2000::/3` | Public, routable |
| Link-local | `fe80::/10` | Valid only on the local link; every interface has one |
| Unique local | `fc00::/7` | IPv6 equivalent of private addresses |
| Multicast | `ff00::/8` | One-to-many |
| Loopback | `::1` | localhost |

### Transition mechanisms
- **Dual stack:** run IPv4 and IPv6 side by side (most common).
- **Tunneling:** wrap IPv6 in IPv4 to cross IPv4-only networks.
- **NAT64/DNS64:** let IPv6-only clients reach IPv4 servers (used by mobile carriers).

**Real-world:** Big Indian mobile operators (like Jio) are largely IPv6-native, and Google/Facebook/Netflix serve content over IPv6 too.

---

## 6. NAT: Network Address Translation ⭐⭐

**Problem:** Not enough public IPv4 addresses; your home has 10 devices but the ISP gives you **one** public IP.

**Solution:** The router rewrites addresses as packets cross between private and public networks.

**Analogy:** A company has one public phone number (reception) and internal extensions. Employees call out through the reception; when the outside person calls back, reception looks at a notebook to know which extension made the call.

### 6.1 Types

| Type | How it works | Use |
|---|---|---|
| **Static NAT** | 1 private IP ↔ 1 fixed public IP | Publishing an internal server |
| **Dynamic NAT** | Private IPs mapped from a pool of public IPs | Rare today |
| **PAT / NAPT / "NAT overload"** | Many private IPs share **one public IP**, distinguished by **port numbers** | **Home routers, most common** |

### 6.2 How PAT works (step by step)

```
Laptop 192.168.1.10:51000  -> router (public 49.36.10.5)  -> Google 142.250.x.x:443

NAT table in router:
  Inside (private)           Outside (public)             Destination
  192.168.1.10:51000   <->   49.36.10.5:60001    <->      142.250.x.x:443
  192.168.1.11:51000   <->   49.36.10.5:60002    <->      142.250.x.x:443
```
1. Outgoing packet: router replaces source `192.168.1.10:51000` with `49.36.10.5:60001` and remembers the mapping.
2. Reply arrives at `49.36.10.5:60001`; router looks up the table and rewrites the destination back to `192.168.1.10:51000`.

### 6.3 Consequences of NAT ⭐
- **Pros:** saves public IPs, hides internal topology, gives a basic "unsolicited inbound blocked" effect.
- **Cons:** breaks the end-to-end principle, complicates P2P, VoIP, online gaming, and protocols that embed IPs in payloads (FTP active mode, SIP). Solutions: **port forwarding**, **UPnP**, **STUN/TURN/ICE** (WebRTC), **hole punching**.
- NAT is **not a security feature by itself**; a stateful firewall is.

### 6.4 CGNAT (Carrier-grade NAT)
ISPs run NAT at scale: hundreds of customers share one public IP (addresses in `100.64.0.0/10`). This is why you sometimes can't host a server at home or port-forward with some ISPs.

### 6.5 NAT in the DevOps world
- **AWS NAT Gateway:** lets instances in *private subnets* reach the Internet (e.g., download updates) without being reachable from outside.
- **Kubernetes/Docker:** container traffic leaving the host is usually SNAT'd (masqueraded) to the host's IP.
- **Source NAT (SNAT)** rewrites source; **Destination NAT (DNAT)** rewrites destination (port forwarding, load balancers).

---

## 7. DHCP: Dynamic Host Configuration Protocol ⭐

**Problem:** Manually configuring IP, mask, gateway and DNS on every device is painful and error-prone.

**Solution:** A DHCP server automatically leases configuration to clients.

**Analogy:** A hotel front desk gives every new guest a room number (IP), tells them where the lift is (gateway) and where to ask for information (DNS), for the duration of their stay (lease).

### What DHCP provides
IP address, subnet mask, **default gateway**, **DNS servers**, lease time (and optionally NTP, domain name, etc.).

### The DORA process ⭐

```
Client                                            DHCP Server
  | ---- 1. DISCOVER (broadcast: "Any DHCP server?") -->  |
  | <--- 2. OFFER (unicast/broadcast: "Take 192.168.1.50")|
  | ---- 3. REQUEST (broadcast: "I accept that offer")-->  |
  | <--- 4. ACK ("Confirmed. Lease = 24 h")              |
```
- Uses **UDP**: server port **67**, client port **68**.
- The client has no IP yet, so it uses source `0.0.0.0` and destination `255.255.255.255` (broadcast). This is also why DHCP can't cross routers by default.
- **DHCP Relay Agent (ip helper):** a router forwards broadcasts to a DHCP server in another subnet, so one server can serve many VLANs.
- **Lease renewal:** at 50% of lease time the client tries to renew (T1); at ~87.5% it tries any server (T2).
- **DHCP reservation:** bind a specific IP to a MAC (printers, servers).
- **If DHCP fails** → Windows/Linux may assign an `169.254.x.x` APIPA address.

### Security issues
- **Rogue DHCP server:** attacker hands out wrong gateway/DNS → traffic hijacking. Defense: **DHCP snooping** on switches.
- **DHCP starvation:** attacker exhausts the IP pool.

---

## 8. ICMP: Internet Control Message Protocol ⭐

ICMP is the network's **error-reporting and diagnostic** protocol. It rides directly inside IP (protocol number 1). It has no ports and is not used to carry user data.

### Common message types

| Type | Meaning | Where you see it |
|---|---|---|
| 0 | Echo Reply | `ping` response |
| 8 | Echo Request | `ping` |
| 3 | Destination Unreachable (network/host/port unreachable, fragmentation needed) | Service down, blocked port |
| 11 | Time Exceeded (TTL hit 0) | `traceroute` |
| 5 | Redirect | Router suggests a better gateway |

### 8.1 How `ping` works
Sends ICMP Echo Requests and measures the time until Echo Reply → gives **RTT** and **packet loss**. Tells you whether the host is reachable at Layer 3.

*If ping fails, it doesn't always mean the host is down: firewalls often block ICMP.*

### 8.2 How `traceroute` works ⭐
Goal: list every router hop on the path.

1. Send a probe with **TTL = 1**. The first router decrements it to 0, drops it and returns **ICMP Time Exceeded** → you learn hop 1's IP and latency.
2. Send probe with **TTL = 2** → second router replies. Continue increasing TTL.
3. When the probe reaches the destination, it replies (ICMP Echo Reply, or "Port Unreachable" for UDP-based probes) and the trace ends.

- Linux `traceroute` uses UDP probes by default; Windows `tracert` uses ICMP echo; `mtr` combines ping+traceroute continuously.
- Stars (`* * *`) mean that router doesn't reply (firewall/rate limit), not necessarily a failure.

**Analogy:** Sending a series of letters with "expires after N stops" labels; each stop that finds the label expired writes back "I'm stop #N". This way you draw the map of the journey.

---

## 9. Router, Switch, Gateway: Clean Differences ⭐

| | Switch | Router | Gateway |
|---|---|---|---|
| Layer | 2 (L3 for L3 switches) | 3 | Often 4-7 |
| Uses | MAC address | IP address | Protocol/format rules |
| Connects | Devices in same network | Different networks | Networks using different protocols/architectures |
| Table | MAC table | Routing table | - |
| Broadcast | Forwards broadcasts | Blocks broadcasts | - |
| Example | Office switch | Home Wi-Fi router | Email gateway, API gateway, **default gateway** (the router that leads out of your subnet) |

Note: "Default gateway" in host settings simply means "the router I send all non-local traffic to".

---

## 10. Putting it together: Host decides "local or remote?"

When your PC sends a packet, it does an **AND** of the destination IP with its own subnet mask:
- If destination is in **my subnet** → ARP for the destination's MAC and send directly.
- If **not** → send to the **default gateway** (ARP for the gateway's MAC), IP destination unchanged.

This is the single most important decision every host makes.

---

## 11. Useful Commands

| Command | Purpose |
|---|---|
| `ip a` / `ipconfig` | Show IP addresses |
| `ip route` / `route print` | Show routing table |
| `ping <host>` | Reachability + RTT |
| `traceroute` / `tracert` / `mtr` | Path discovery |
| `arp -a` / `ip neigh` | ARP cache |
| `nslookup` / `dig` | DNS queries |
| `ss -tulpn` / `netstat -an` | Open ports and connections |

---

## 12. Interview Q&A

**Q1. Public vs private IP?** ⭐ Section 2.2.

**Q2. What is subnetting, why do we do it?** ⭐
Splitting a large network into smaller ones for security, performance (smaller broadcast domains) and efficient address use. Use CIDR notation (`/24`).

**Q3. What is a subnet mask / CIDR?**
A mask marks which bits are network vs host. CIDR writes it as a prefix length (/24), and permits classless allocation and route aggregation.

**Q4. IPv4 vs IPv6?** ⭐
32-bit vs 128-bit; broadcast vs no broadcast; NAT-dependent vs NAT-free; ARP vs NDP; variable header with checksum vs fixed simpler header; fragmentation by routers vs only sender.

**Q5. What is NAT and how does PAT work?** ⭐⭐ Section 6.2. Mention port numbers and translation table.

**Q6. How does DHCP work?** ⭐ DORA, UDP 67/68, lease, relay agent.

**Q7. What is TTL and why is it important?**
A hop counter; routers decrement it; drops packet at 0 and sends ICMP Time Exceeded. Prevents infinite loops and enables traceroute.

**Q8. How does traceroute work?** ⭐ Section 8.2.

**Q9. Why is ICMP sometimes blocked, and what breaks?**
For security (recon, ping floods). But blocking all ICMP breaks Path MTU Discovery and IPv6 NDP, causing hangs and failures.

**Q10. IP is unreliable: what does that mean?**
IP gives best-effort delivery: packets can be lost, duplicated, delayed or reordered. TCP (or the application) adds reliability.

**Q11. My laptop shows 169.254.x.x. What happened?**
DHCP failed; the OS self-assigned an APIPA address. Check cable/Wi-Fi, DHCP server, VLAN or relay.

**Q12. How does a host decide whether to use the gateway?**
It compares the destination with its own network using the mask. Same subnet → direct via ARP. Otherwise → default gateway.

**Q13. Difference between NAT gateway and internet gateway in AWS?**
Internet Gateway gives two-way Internet access to public subnets (public IPs). NAT Gateway lets private-subnet instances start outbound connections only.

**Q14. What is CGNAT and its effect on hosting?**
ISP-level NAT sharing one public IP among many customers, so inbound connections/port forwarding to your home don't work.

---

## 13. Cheat Sheet

- IPv4 = 32 bit, IPv6 = 128 bit. Private ranges: 10/8, 172.16/12, 192.168/16. Loopback 127.0.0.1. APIPA 169.254/16.
- Hosts per subnet = 2^host-bits − 2.
- `/24` = 255.255.255.0, `/16` = 255.255.0.0, `/8` = 255.0.0.0. Larger prefix → smaller subnet.
- IP = best effort. TTL stops loops. Protocol field: 6 TCP, 17 UDP, 1 ICMP.
- PAT = many private → one public using ports.
- DHCP = DORA, UDP 67/68, gives IP + mask + gateway + DNS.
- ICMP: ping (echo 8/0), traceroute (TTL + Time Exceeded 11), Destination Unreachable 3.
- IPv6: no broadcast, no ARP (NDP), SLAAC, link-local `fe80::`.
