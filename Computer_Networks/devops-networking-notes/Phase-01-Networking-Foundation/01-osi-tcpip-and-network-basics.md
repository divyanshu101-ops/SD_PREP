# Phase 1 · File 1: Network Basics, OSI Model and TCP/IP Model

> **Goal of this file:** understand what a network is, how its performance is measured, why we split networking into layers, and how a piece of data travels down the layers on the sender and up the layers on the receiver.

---

## 1. What is a Computer Network?

A computer network is a set of devices (computers, phones, servers, routers) connected so they can **exchange data** and **share resources** (files, printers, internet).

**Analogy:** A network is a road system. Devices are houses, links (cables, Wi-Fi) are roads, and data is the traffic. Rules (protocols) are traffic laws that everyone follows so that there are no crashes.

### Key vocabulary

| Term | Meaning | Analogy |
|---|---|---|
| **Node / Host** | Any device on the network | A house |
| **Link** | Connection between two nodes | A road |
| **Protocol** | Agreed set of rules for communication | Traffic law / language |
| **Packet** | A small chunk of data with an address label | A parcel |
| **Client** | Device that asks for a service | Customer |
| **Server** | Device that provides a service | Shop |
| **Port** | A numbered door on a host for a specific app | Room number in a building |

---

## 2. Types of Networks by Size

| Type | Full form | Range | Example |
|---|---|---|---|
| **PAN** | Personal Area Network | ~1-10 m | Phone ↔ Bluetooth earbuds |
| **LAN** | Local Area Network | One building / campus | College Wi-Fi, home router |
| **MAN** | Metropolitan Area Network | A city | A cable TV / city-wide ISP network |
| **WAN** | Wide Area Network | Countries, continents | The Internet, a bank's network across cities |

**Real-world:** Your college hostel Wi-Fi is a LAN. Your ISP connects that LAN to the wider Internet, which is a giant WAN made of many WANs joined together (that is why the Internet is called a "network of networks").

---

## 3. Network Topologies (how devices are arranged)

| Topology | Shape | Pros | Cons | Real use |
|---|---|---|---|---|
| **Bus** | All devices on one shared cable | Cheap | One cable break kills everything; collisions | Old Ethernet (obsolete) |
| **Ring** | Devices in a circle | Orderly access | One break can break the ring | Old Token Ring, SONET rings |
| **Star** | Everyone connects to a central switch | Easy to add/remove devices, one failure doesn't kill others | Central device is a single point of failure | **Almost all modern LANs** |
| **Mesh (full/partial)** | Many direct links between devices | Very reliable, multiple paths | Expensive, complex | The Internet core, data-center fabrics |
| **Tree/Hybrid** | Stars connected in a hierarchy | Scalable | Root failure hurts | Large campus networks |

**Analogy:** Star topology = airline hub (all flights go through one hub airport). Mesh = a city with many roads between every neighborhood.

**Interview line:** "Modern LANs use star topology with a switch at the center. The Internet backbone is a partial mesh so that traffic can reroute around failures."

---

## 4. Performance Metrics (⭐ very commonly asked)

### 4.1 Bandwidth
The **maximum** amount of data a link can carry per second (e.g., 100 Mbps). It is the *capacity* of the pipe.

### 4.2 Throughput
The **actual** data rate you achieve. Always ≤ bandwidth because of overhead, congestion, retransmissions.

### 4.3 Latency
The **time** it takes for a packet to travel from source to destination (one way). Measured in milliseconds.

Latency has four components:
1. **Propagation delay:** signal travel time (limited by the speed of light in the medium; Kolkata → US is physically far, so this can't be removed).
2. **Transmission delay:** time to push all bits of the packet onto the wire (depends on packet size and bandwidth).
3. **Processing delay:** routers/switches examining the header.
4. **Queuing delay:** waiting in a router's buffer because the link is busy.

### 4.4 RTT (Round Trip Time)
Time for a packet to go to the destination and the reply to come back. `ping` measures RTT.

### 4.5 Jitter
The **variation** in latency between packets. Critical for voice/video calls.

### 4.6 Packet loss
Percentage of packets that never arrive (dropped due to congestion, bad links, wireless interference).

### The classic analogy: water pipe vs highway

- **Bandwidth** = how wide the highway is (number of lanes).
- **Latency** = how long the trip takes for one car.
- **Throughput** = cars per hour actually reaching the destination.
- **Jitter** = some cars take 10 minutes, others 30 minutes: unpredictable.
- **Packet loss** = cars that break down and never arrive.

> **Key insight:** A wide highway (high bandwidth) does not make a single car faster (low latency). A satellite link can have huge bandwidth but 600 ms latency. Gamers care about latency; video downloaders care about bandwidth.

### Bandwidth-Delay Product (concept only)
`BDP = bandwidth × RTT` is the amount of data "in flight" on the wire at any moment. It explains why long, fat links need large TCP windows to be used fully. (No numbers needed; know the idea.)

---

## 5. Switching Techniques

### 5.1 Circuit switching
A dedicated path is reserved for the whole conversation (old telephone network). Guaranteed quality but wasteful when idle.

### 5.2 Packet switching (the Internet)
Data is chopped into packets. Each packet is routed independently and may take a different path. Packets are reassembled at the destination.

| | Circuit switching | Packet switching |
|---|---|---|
| Path | Fixed, reserved | Independent per packet |
| Efficiency | Wasteful when idle | Very efficient (shared links) |
| Setup | Required | Not required (for datagram) |
| Example | Landline call | Internet |

**Analogy:** Circuit switching = booking a whole train for yourself. Packet switching = sending your luggage in many shared courier vans; each van may take a different road.

**Why the Internet chose packet switching:** it is efficient, robust (traffic reroutes around failures) and lets thousands of users share the same links.

---

## 6. Why do we need Layers?

Networking is very complex: electrical signals, addressing, routing, reliability, encryption, applications... Putting everything in one giant program would be unmanageable.

**Layering** divides the problem into small parts. Each layer:
- Does **one job**.
- Uses services of the layer **below**.
- Provides services to the layer **above**.
- Can be **changed independently** (you can switch from Wi-Fi to Ethernet and your browser doesn't care).

**Analogy: sending a gift by courier**
1. You write a letter (application).
2. You put it in an envelope and write the address (transport/network).
3. The courier company puts it in a van (data link).
4. The van drives on the road (physical).
At the other end the same steps are undone in reverse order.

---

## 7. The OSI Model (7 Layers) ⭐

OSI = **Open Systems Interconnection**, made by ISO as a *reference/conceptual* model. Memory trick (top to bottom): **"All People Seem To Need Data Processing"** = Application, Presentation, Session, Transport, Network, Data Link, Physical.

```
+---------------------+
| 7. Application      |  <- what the user/app sees (HTTP, DNS, SMTP)
| 6. Presentation     |  <- format, encryption, compression
| 5. Session          |  <- start / maintain / end conversations
| 4. Transport        |  <- end-to-end delivery (TCP, UDP)  [Segment]
| 3. Network          |  <- logical addressing + routing (IP) [Packet]
| 2. Data Link        |  <- node-to-node delivery in one network (MAC) [Frame]
| 1. Physical         |  <- raw bits on the wire / air        [Bits]
+---------------------+
```

### Layer 1: Physical
- **Job:** Transmit raw **bits** (0/1) as electrical, light or radio signals.
- **Deals with:** cables (Cat6, fiber), connectors, voltage levels, Wi-Fi radio frequencies, bit rate.
- **Devices:** hubs, repeaters, cables, NICs (partly).
- **Analogy:** The actual road and the trucks' wheels.

### Layer 2: Data Link
- **Job:** Reliable delivery of **frames** between two devices on the **same** network (hop-to-hop). Adds **MAC addresses**, does error detection.
- **Protocols/tech:** Ethernet, Wi-Fi (802.11), ARP (bridges L2/L3), PPP, VLAN.
- **Devices:** switches, bridges, NIC.
- **Analogy:** The delivery person who knows the exact flat number inside one building.

### Layer 3: Network
- **Job:** **Logical addressing (IP)** and **routing** packets across different networks, from source to destination host.
- **Protocols:** IP (IPv4/IPv6), ICMP, OSPF, BGP, IPsec.
- **Devices:** routers, L3 switches.
- **Analogy:** Postal sorting office that chooses which city the parcel goes to next.

### Layer 4: Transport
- **Job:** **End-to-end** communication between **processes** (applications) using **ports**. Provides reliability (TCP) or speed (UDP), flow control, congestion control, segmentation.
- **Protocols:** TCP, UDP, (QUIC runs over UDP).
- **Analogy:** The receptionist at the destination building who hands the parcel to the right department (port) and confirms delivery.

### Layer 5: Session
- **Job:** Establish, manage and terminate **sessions** (dialogues). Checkpointing/synchronization.
- **Examples:** RPC sessions, NetBIOS, session tokens conceptually.
- **Reality check:** In TCP/IP this layer is merged into the application; you rarely see it as a separate thing.

### Layer 6: Presentation
- **Job:** **Translation, encryption, compression** so both sides understand the data format.
- **Examples:** TLS/SSL (often placed here conceptually), JPEG, MPEG, ASCII/UTF-8, JSON/XML serialization.
- **Analogy:** A translator + a security guard who seals the envelope.

### Layer 7: Application
- **Job:** Provides network services directly to **applications/users**.
- **Protocols:** HTTP/HTTPS, DNS, FTP, SMTP, IMAP, SSH, DHCP.
- **Important:** The "application layer" is not the app (Chrome) itself. It is the *protocol* the app uses (HTTP).

### One-line summary table

| # | Layer | PDU | Addressing | Key protocols | Devices |
|---|---|---|---|---|---|
| 7 | Application | Data | - | HTTP, DNS, SMTP, FTP | Gateways, L7 LB |
| 6 | Presentation | Data | - | TLS, JPEG, UTF-8 | - |
| 5 | Session | Data | - | RPC, NetBIOS | - |
| 4 | Transport | Segment (TCP) / Datagram (UDP) | Port | TCP, UDP | L4 LB, firewalls |
| 3 | Network | Packet | IP address | IP, ICMP, OSPF, BGP | Router, L3 switch |
| 2 | Data Link | Frame | MAC address | Ethernet, Wi-Fi, ARP | Switch, bridge, NIC |
| 1 | Physical | Bits | - | Ethernet PHY, USB | Hub, cable, repeater |

---

## 8. The TCP/IP Model (4 layers) ⭐

The OSI model is a teaching model. The **TCP/IP model** is what the Internet actually uses.

| TCP/IP layer | Covers OSI layers | Examples |
|---|---|---|
| **Application** | 7 + 6 + 5 | HTTP, HTTPS, DNS, SMTP, SSH, DHCP, FTP |
| **Transport** | 4 | TCP, UDP |
| **Internet** | 3 | IP, ICMP, IPsec |
| **Network Access (Link)** | 2 + 1 | Ethernet, Wi-Fi, ARP |

Many textbooks use a **5-layer hybrid** (Application, Transport, Network, Data Link, Physical). That is the most practical one to remember.

### OSI vs TCP/IP (⭐ classic question)

| Point | OSI | TCP/IP |
|---|---|---|
| Layers | 7 | 4 (or 5) |
| Nature | Theoretical reference model | Practical, implemented on the Internet |
| Developed by | ISO | US DoD (ARPANET) |
| Approach | Protocol-independent, "model first, protocols later" | Protocols first, model derived from them |
| Session/Presentation | Separate layers | Merged into Application |
| Strictness | Strict layering | More flexible |

---

## 9. Encapsulation and Decapsulation ⭐

**Encapsulation** = each layer, on the way *down* at the sender, **wraps** the data from the layer above with its own **header** (and sometimes a trailer).
**Decapsulation** = on the receiving side, each layer *removes* its header and passes the payload up.

```
SENDER                                      RECEIVER
Application:  [ DATA ]                      [ DATA ]  <- app reads it
Transport:    [TCP hdr][ DATA ]             strip TCP header
Network:      [IP hdr][TCP hdr][ DATA ]     strip IP header
Data Link:    [Eth hdr][IP][TCP][DATA][FCS] strip Ethernet header/trailer
Physical:     010101010101...  ------------>  bits received
```

**Analogy: Russian nesting dolls / envelope in envelope.** Your letter goes into an envelope with a flat number, that goes into a bigger envelope with a city address, that goes into a sack labeled for the truck. The receiver opens the sack, then the big envelope, then the small envelope.

### What each header contains (idea only)

- **TCP header:** source port, destination port, sequence number, ACK number, flags (SYN/ACK/FIN), window size.
- **IP header:** source IP, destination IP, TTL, protocol (TCP=6, UDP=17, ICMP=1).
- **Ethernet header:** source MAC, destination MAC, EtherType (IPv4/IPv6/ARP).

### Important nuance ⭐
While a packet travels across the Internet:
- **Source and destination IP stay the same** (end to end), except when NAT changes them.
- **Source and destination MAC change at every hop** (each link has its own frame).

**Analogy:** The final address on the parcel (IP) stays the same, but at every stage of the journey a new delivery slip (MAC) with "from this truck to that truck" is stuck on.

---

## 10. Full worked example: What happens when you send a WhatsApp/HTTP message?

Let's take a simple HTTP request from your laptop to a server.

1. **Application:** Browser creates an HTTP GET request (`GET /index.html`).
2. **Transport:** TCP splits data into segments if needed, adds source port (random, e.g., 52344) and destination port (80 or 443).
3. **Network:** IP adds your IP and the server's IP. Decides whether the destination is local or must go via the **default gateway** (your router).
4. **Data Link:** Ethernet/Wi-Fi adds your MAC and the **router's MAC** (found using ARP) as destination MAC.
5. **Physical:** Bits/radio waves leave your laptop.
6. The router removes the frame, reads the IP header, decides the next hop, creates a **new frame** with new MAC addresses, and forwards.
7. This repeats across many routers until the server's network. The server's NIC receives the frame, and each layer peels its header until the web server application gets the HTTP request.

---

## 11. Devices and the layers they operate on

| Device | Layer | What it looks at | Job |
|---|---|---|---|
| **Repeater / Hub** | 1 | Electrical signal | Regenerates/broadcasts bits to all ports (dumb) |
| **Bridge / Switch** | 2 | MAC address | Forwards frames only to the correct port |
| **Router** | 3 | IP address | Connects different networks, chooses best path |
| **L3 Switch** | 2+3 | MAC + IP | Switch with routing capability |
| **Firewall** | 3-7 | IP, port, sometimes app data | Filters traffic |
| **Load Balancer (L4/L7)** | 4 / 7 | IP+port / HTTP content | Distributes traffic across servers |
| **Gateway** | Any (often 7) | Protocol translation | Connects networks that use different protocols |

---

## 12. Communication Models

- **Client-Server:** Clients request, servers respond (web, email, most backends). Centralized, easy to manage, server can be a bottleneck.
- **Peer-to-Peer (P2P):** Every node is both client and server (BitTorrent, some blockchain networks). Scales naturally, harder to control.

**Unicast / Multicast / Broadcast / Anycast** (short recap, detailed in Phase 2):
- Unicast = one to one. Multicast = one to a group. Broadcast = one to all in the local network. Anycast = one to the *nearest* of many.

---

## 13. Standards and organizations (good to know)

- **IETF:** publishes RFCs (rules for Internet protocols, e.g., RFC 793 = TCP).
- **IEEE:** defines LAN standards (802.3 = Ethernet, 802.11 = Wi-Fi).
- **ISO:** OSI model.
- **IANA/ICANN:** manage IP address allocation, port numbers, DNS root.

---

## 14. Common Misconceptions

1. "Bandwidth = speed." No. Bandwidth is capacity; latency is delay.
2. "HTTPS is a separate layer." No, it is HTTP on top of TLS on top of TCP.
3. "The OSI model is used on the Internet." The Internet uses TCP/IP; OSI is used to *talk about* networking.
4. "MAC addresses are used across the Internet." MAC is only meaningful within one local link.
5. "Packets always follow the same path." In packet switching, they may not.

---

## 15. Q&A

**Q1. Explain the OSI model briefly.** ⭐
A seven-layer reference model. From top: Application (user-facing protocols), Presentation (format/encryption), Session (dialog control), Transport (end-to-end delivery with ports), Network (IP addressing and routing), Data Link (hop-to-hop delivery with MAC), Physical (bits on the medium). It helps us divide networking into independent, replaceable parts and to troubleshoot layer by layer.

**Q2. OSI vs TCP/IP?** ⭐ Use the table in Section 8.

**Q3. What is encapsulation?**
Each layer adds its header to the data from the layer above as it travels down. The receiver removes them in reverse (decapsulation).

**Q4. What is the PDU at each layer?**
Data (L5-7), Segment/Datagram (L4), Packet (L3), Frame (L2), Bits (L1).

**Q5. Difference between bandwidth and latency?**
Bandwidth is how much data per second the link can carry; latency is how long a bit takes to arrive. A truck is high bandwidth, high latency; a sports car is low bandwidth, low latency.

**Q6. Which layer does a router / switch / hub work at?**
Router L3, switch L2, hub L1.

**Q7. Why layered architecture?**
Modularity, independent evolution of layers, easier troubleshooting, interoperability between vendors.

**Q8. Does the MAC address change while a packet travels across the Internet?**
Yes, at every hop. The IP addresses stay the same end to end (unless NAT).

**Q9. Circuit vs packet switching?**
See table in Section 5. Internet = packet switching.

**Q10. What is jitter and why does it matter?**
Variation in packet delay. It hurts real-time apps like VoIP, video calls and gaming; solved partly with jitter buffers.

**Q11. If a website doesn't load, how do you troubleshoot using the layers?**
Bottom up: cable/Wi-Fi connected (L1) → link/ARP/IP address obtained (L2/L3) → ping the gateway and 8.8.8.8 (L3) → DNS resolves (L7) → port reachable via `curl`/`telnet` (L4) → HTTP status codes (L7).

---

## 16. Cheat Sheet

- OSI (7): **A**ll **P**eople **S**eem **T**o **N**eed **D**ata **P**rocessing.
- TCP/IP (4): Application, Transport, Internet, Network Access.
- PDUs: Data → Segment → Packet → Frame → Bits.
- Addressing: Port (L4), IP (L3), MAC (L2).
- Hub L1, Switch L2, Router L3.
- IP is end-to-end, MAC is hop-to-hop.
- Bandwidth = capacity, Throughput = actual, Latency = delay, Jitter = variation of delay.
- Internet = packet switching, star LANs, mesh core.
