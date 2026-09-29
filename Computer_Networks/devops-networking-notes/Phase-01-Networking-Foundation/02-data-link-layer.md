# Phase 1 · File 2: Physical and Data Link Layer

> **Goal:** understand how devices in the *same local network* talk to each other: MAC addresses, Ethernet frames, hubs vs switches, ARP, VLANs, loop prevention (STP) and error detection.

**Analogy for this whole file:** Imagine one large apartment building. The **data link layer** is everything that happens *inside* the building: the security desk that knows which flat (MAC address) is on which floor (switch port), the lift, and the corridor rules. It does not know how to reach other cities; that is the network layer's job.

---

## 1. Physical Layer Essentials

### 1.1 Transmission media

| Medium | Type | Notes |
|---|---|---|
| **Twisted pair (Cat5e/Cat6)** | Copper, wired | Common in LANs, up to ~100 m per segment |
| **Coaxial** | Copper | Cable TV / broadband |
| **Fiber optic** | Light through glass | Very high speed, long distance, immune to electrical noise; used in ISP backbones and undersea cables |
| **Wireless (Wi-Fi, Bluetooth, cellular)** | Radio waves | Convenient but shared, interference-prone |

### 1.2 Basic concepts
- **Signal encoding:** turning bits into voltage/light/radio patterns.
- **Simplex / Half-duplex / Full-duplex:**
  - Simplex = one direction only (TV broadcast).
  - Half-duplex = both directions but not at the same time (walkie-talkie, old hubs).
  - Full-duplex = both at once (phone call, modern switched Ethernet).
- **Attenuation:** signal weakens with distance (repeaters boost it).
- **Noise / interference:** unwanted signals corrupt bits.

**Real-world:** Undersea fiber cables carry most of the world's Internet traffic between continents (India connects to Europe/US through cables landing at Mumbai, Chennai etc.).

---

## 2. Data Link Layer: Responsibilities

1. **Framing:** wrap network-layer packets into frames with start/end markers.
2. **Physical addressing (MAC):** identify sender/receiver on the local link.
3. **Error detection:** detect corrupted frames (CRC).
4. **Media access control:** decide who may transmit when the medium is shared.
5. **Flow control (basic)** between two directly connected nodes.

It has two sublayers:
- **LLC (Logical Link Control):** talks to the network layer above.
- **MAC (Media Access Control):** talks to the physical medium below.

---

## 3. MAC Address ⭐

A **MAC (Media Access Control) address** is a hardware identifier burned into a network interface card (NIC). It is **48 bits (6 bytes)** written in hex: `3C:52:82:1A:B4:9F`.

```
3C:52:82   :   1A:B4:9F
   OUI          Device-specific part
(vendor id)     (assigned by vendor)
```

- **OUI (first 3 bytes):** identifies the manufacturer (Intel, Apple, Cisco...).
- **Last 3 bytes:** unique number assigned by the manufacturer.
- **Broadcast MAC:** `FF:FF:FF:FF:FF:FF` means "everyone on this local network".
- **Multicast MAC:** starts with an odd first byte (the lowest bit of the first byte is 1).

### Facts to remember
- MAC addresses are **flat** (no hierarchy for routing). IP is hierarchical.
- Scope is **local only**. Routers do not forward MAC addresses; they rewrite them at every hop.
- Can be **spoofed** by software (used in MAC filtering bypass). Modern phones use **randomized MACs** for privacy on Wi-Fi.

### MAC vs IP ⭐

| | MAC | IP |
|---|---|---|
| Layer | 2 | 3 |
| Length | 48-bit | 32-bit (IPv4) / 128-bit (IPv6) |
| Assigned | By manufacturer (can change in software) | By network admin / DHCP |
| Scope | Local network segment | Global (routable) |
| Changes when you move networks? | No | Yes |

**Analogy:** MAC = your **name/Aadhaar number** (identifies you, moves with you). IP = your **current postal address** (changes when you move house).

---

## 4. Ethernet Frame Format

```
+----------+-----+-----------+-----------+----------+-----------------+-----+
| Preamble | SFD | Dest MAC  | Src MAC   | EtherType| Payload (data)  | FCS |
|  7 B     | 1 B |   6 B     |   6 B     |   2 B    |  46 - 1500 B    | 4 B |
+----------+-----+-----------+-----------+----------+-----------------+-----+
```

- **Preamble + SFD:** synchronization pattern so receiver clocks lock on.
- **EtherType:** tells what is inside (0x0800 = IPv4, 0x86DD = IPv6, 0x0806 = ARP).
- **Payload:** the IP packet. Minimum 46 bytes (padded if smaller).
- **FCS (Frame Check Sequence):** a CRC value to detect corruption.
- **MTU (Maximum Transmission Unit):** max payload size, **1500 bytes** for standard Ethernet. Bigger IP packets must be fragmented or sized down.
- **Jumbo frames:** up to ~9000 bytes, used inside data centers/storage networks to reduce overhead.

---

## 5. Collision Domain and Broadcast Domain ⭐

- **Collision domain:** a zone where two devices sending at the same time will collide.
- **Broadcast domain:** the set of devices that receive each other's broadcast frames.

| Device | Splits collision domains? | Splits broadcast domains? |
|---|---|---|
| Hub | No (one big collision domain) | No |
| Switch | **Yes** (each port is its own collision domain) | No (unless VLANs) |
| Router | Yes | **Yes** (each interface = separate broadcast domain) |

**Analogy:** A hub is a **room where everybody shouts** and everybody hears everything. A switch is a **receptionist who whispers each message only to the intended person**. A router is a **wall between two different rooms**: a shout in one room is not heard in the other.

---

## 6. Media Access Control: CSMA/CD and CSMA/CA

When many devices share one medium, we need rules.

### CSMA/CD (Carrier Sense Multiple Access with Collision Detection): old wired Ethernet with hubs
1. **Listen** to see if the line is quiet (carrier sense).
2. If quiet, **transmit**.
3. While transmitting, **listen for collision**.
4. If collision detected, send a jam signal, stop, wait a **random back-off time**, retry.

Analogy: people in a meeting; if two start speaking at once, both stop and try again after a random pause.

Today, switched full-duplex Ethernet has **no collisions**, so CSMA/CD is essentially historical.

### CSMA/CA (Collision Avoidance): Wi-Fi
Wireless devices cannot listen while they transmit, so they *avoid* collisions:
1. Listen; if the channel is idle, wait a small random back-off.
2. Send the frame. Receiver replies with an **ACK**.
3. Optionally use **RTS/CTS** (Request to Send / Clear to Send) to reserve the channel and solve the **hidden node problem** (two clients cannot hear each other but both reach the access point).

---

## 7. Hub vs Bridge vs Switch ⭐

### Hub (Layer 1)
Receives bits on one port and **repeats to all other ports**. No intelligence, half-duplex, collisions, security risk (anyone can sniff).

### Bridge (Layer 2)
Connects two LAN segments and filters by MAC address. The ancestor of the switch, usually 2-4 ports, software-based.

### Switch (Layer 2) ⭐
A multi-port bridge in hardware. Learns which MAC address is on which port and forwards frames **only** to the correct port.

**How a switch works: the MAC address table (CAM table)**

1. **Learning:** When a frame arrives on a port, the switch reads the **source MAC** and records "MAC X is reachable via port N".
2. **Forwarding:** It looks up the **destination MAC**.
   - If found in the table → send only out that port.
   - If destination MAC is on the same port it arrived from → **filter** (drop).
3. **Flooding:** If the destination MAC is **unknown** (or broadcast/multicast) → send out **all ports except the incoming one**.
4. **Aging:** Entries expire after some time (commonly around 300 seconds) if not refreshed.

Walkthrough:
```
A(port1)  B(port2)  C(port3)

1. A sends frame to B. Switch table empty.
   -> learns "A is on port1"; B unknown -> floods to port 2 and 3.
2. B replies to A.
   -> learns "B is on port2"; A is known -> forwards only to port 1.
3. Now A<->B traffic is private; C sees nothing.
```

### Types of switching methods (concept)
- **Store-and-forward:** receive the full frame, check CRC, then forward (most common; catches errors).
- **Cut-through:** start forwarding as soon as the destination MAC is read (lower latency, may forward bad frames).

### Managed vs unmanaged switch
Unmanaged = plug and play. Managed = configurable (VLANs, port security, monitoring, STP).

---

## 8. ARP: Address Resolution Protocol ⭐⭐

**Problem:** Your computer knows the **IP** of the destination (or the gateway) but the Ethernet frame needs a **MAC**. How to find the MAC for a given IP on the local network?

**Solution: ARP.**

### ARP process step by step
Your PC `192.168.1.10` wants to talk to the router `192.168.1.1`:

1. PC checks its **ARP cache** (`arp -a`). Not found.
2. PC sends an **ARP Request** as a **broadcast** (dest MAC `FF:FF:FF:FF:FF:FF`): *"Who has 192.168.1.1? Tell 192.168.1.10."*
3. Every device on the LAN receives it, but only the owner of that IP replies.
4. The router sends an **ARP Reply** (unicast): *"192.168.1.1 is at AA:BB:CC:DD:EE:FF."*
5. PC stores it in its ARP cache (for a few minutes) and sends the frame.

**Analogy:** You stand in a hall and shout, *"Who is Rahul from flat 101?"* Rahul raises his hand and says, "I'm here, this is my door." You remember it for a while.

### Important ARP facts
- ARP works **only within a local network** (one broadcast domain).
- For a destination on another network, the PC ARPs for the **default gateway's MAC**, not the remote server's MAC. ⭐
- ARP has **no authentication** → vulnerable to **ARP spoofing/poisoning** (attacker replies with its own MAC to become man-in-the-middle). Defenses: Dynamic ARP Inspection, static ARP for critical hosts, port security.
- **Gratuitous ARP:** a host announces its own IP-MAC mapping (used when IP changes, failover in high-availability setups, duplicate IP detection).
- **Proxy ARP:** a router answers ARP on behalf of another network's host (legacy use).
- **RARP:** reverse (MAC → IP), obsolete; replaced by DHCP.
- IPv6 doesn't use ARP; it uses **NDP (Neighbor Discovery Protocol)** through ICMPv6.

---

## 9. VLAN: Virtual LAN ⭐

**Problem:** One big switch → one big broadcast domain. Everyone (HR, Finance, Guests) shares broadcasts and can sniff each other. Buying separate switches for each department is expensive.

**Solution:** A VLAN logically splits one physical switch into **multiple virtual switches**, each with its own broadcast domain.

**Analogy:** One large office floor divided by soundproof glass walls into separate rooms (HR room, Finance room, Guest room) without building new floors.

### Concepts
- **VLAN ID:** number (1-4094) identifying the VLAN.
- **Access port:** belongs to exactly one VLAN; connects end devices (PC, printer). Frames are untagged.
- **Trunk port:** carries traffic of **multiple VLANs** between switches (or switch-router). Frames carry a **802.1Q tag** (4 bytes containing the VLAN ID).
- **Native VLAN:** the VLAN whose frames cross the trunk untagged.

```
[HR PC]--\                               /--[HR PC]
[Fin PC]--[Switch A]====trunk====[Switch B]--[Fin PC]
[Guest]--/                               \--[Guest]
     VLAN10 / VLAN20 / VLAN30 travel over one trunk link
```

### Inter-VLAN routing
Devices in different VLANs are in different broadcast domains (and usually different IP subnets), so they need a **router (Layer 3)** to talk:
- **Router-on-a-stick:** one router interface with sub-interfaces per VLAN.
- **L3 switch (SVI):** the switch itself routes between VLANs (most common in enterprises).

### Benefits
Security isolation, smaller broadcast domains (better performance), flexible grouping (people in different floors in the same VLAN), simpler management.

**Real-world:** A college keeps students, faculty, admin, and CCTV cameras in different VLANs so a student laptop cannot reach the admin database directly.

---

## 10. STP: Spanning Tree Protocol (loop prevention) ⭐

**Problem:** For redundancy, engineers connect switches with multiple links. But Ethernet frames have **no TTL**. A broadcast frame will loop forever between switches, multiplying → **broadcast storm** → network dies. Also MAC tables become unstable.

**Solution: STP (IEEE 802.1D).** Switches talk to each other (BPDU messages), elect a **root bridge**, and **block redundant ports** so the topology becomes a loop-free tree. If an active link fails, a blocked port is automatically unblocked.

**Analogy:** A city has many roads between two places, but on a one-way rule day the traffic police close some roads temporarily to avoid endless circling; they reopen a road if another is blocked.

- **Root bridge:** the reference switch (lowest bridge ID).
- **Port roles:** Root port, Designated port, Blocked (alternate) port.
- **Variants:** RSTP (Rapid STP, faster convergence), MSTP (multiple instances per VLAN group).
- **Related:** **Link aggregation / LACP / EtherChannel**: bundle multiple physical links into one logical link for more bandwidth without STP blocking them.

---

## 11. Error Detection and Correction

Bits can flip due to noise. We need to **detect** errors (and sometimes correct).

| Method | Idea | Strength |
|---|---|---|
| **Parity bit** | Add one bit to make the number of 1s even/odd | Detects odd number of bit errors only |
| **Checksum** | Add up data words, send the sum (used in IP/TCP/UDP headers) | Simple, weaker |
| **CRC (Cyclic Redundancy Check)** | Treat data as a polynomial, divide by a generator, append remainder | Very strong, used in Ethernet FCS, Wi-Fi |
| **Hamming code** | Extra bits at power-of-2 positions to locate and fix single-bit errors | Error **correction** |

- **ARQ (Automatic Repeat Request):** if error is detected, ask for retransmission (Stop-and-wait, Go-Back-N, Selective Repeat). TCP uses this idea at L4.
- **FEC (Forward Error Correction):** send extra redundancy so receiver can fix errors without retransmission (used in satellites, Wi-Fi, video streaming).

Interview line: "Ethernet detects errors with CRC and simply **drops** bad frames; it does not retransmit. Reliability is TCP's job above."

---

## 12. Wi-Fi (802.11) Basics

- **SSID:** network name. **AP (Access Point):** connects wireless clients to the wired LAN.
- **Frequency bands:** 2.4 GHz (longer range, slower, crowded), 5 GHz (faster, shorter range), 6 GHz (Wi-Fi 6E/7).
- **Standards:** 802.11n (Wi-Fi 4), ac (Wi-Fi 5), ax (Wi-Fi 6), be (Wi-Fi 7).
- **Security:** WEP (broken) → WPA → WPA2 (AES) → **WPA3** (current).
- **Wi-Fi uses CSMA/CA** and ACKs for every unicast frame.
- **Roaming:** phone moves between APs with the same SSID.

---

## 13. Other Link-layer Ideas Worth Knowing

- **PPP / PPPoE:** used by some broadband ISPs to authenticate users.
- **Port security:** limit MAC addresses per switch port; prevents unauthorized devices.
- **Port mirroring (SPAN):** copy traffic to a monitoring port for Wireshark.
- **PoE (Power over Ethernet):** power delivered on the network cable (IP cameras, access points).
- **MAC flooding attack:** attacker fills the switch's MAC table with fake entries so it starts flooding like a hub (attacker can sniff). Defense: port security.

---

## 14. Interview Q&A

**Q1. What is a MAC address? How is it different from IP?** ⭐ See Section 3 table.

**Q2. How does a switch know where to send a frame?** ⭐
It maintains a MAC address table. It learns source MACs from incoming frames, forwards to the known port for the destination MAC, floods when unknown/broadcast, and ages out old entries.

**Q3. Hub vs switch vs router?** ⭐
Hub repeats bits to all ports (L1, one collision domain). Switch forwards frames by MAC (L2, separate collision domain per port, one broadcast domain). Router forwards packets by IP between networks (L3, separates broadcast domains).

**Q4. Explain ARP.** ⭐ Section 8. Mention broadcast request, unicast reply, cache, and that for remote hosts it resolves the gateway's MAC.

**Q5. What if the destination is outside my network, whose MAC goes in the frame?**
The default gateway's MAC. IP destination stays the remote server.

**Q6. What is a VLAN and why use it?** Section 9. Logical segmentation, security, smaller broadcast domains.

**Q7. Access port vs trunk port?**
Access carries one VLAN untagged to an end device; trunk carries many VLANs tagged with 802.1Q between network devices.

**Q8. What is a broadcast storm and how is it prevented?**
A loop causes frames to circulate and multiply forever. Ethernet has no TTL. STP blocks redundant links to remove loops.

**Q9. What is ARP spoofing and how do you defend?**
Attacker sends fake ARP replies to associate its MAC with another host's IP (like the gateway) to intercept traffic. Defend using Dynamic ARP Inspection, DHCP snooping, static ARP entries, encryption (HTTPS/VPN) so intercepted data is useless.

**Q10. CSMA/CD vs CSMA/CA?**
CD detects collisions and retransmits (wired shared Ethernet, obsolete). CA avoids collisions using back-off, ACKs and optional RTS/CTS (Wi-Fi).

**Q11. What is MTU?**
Largest payload a link can carry in one frame (1500 bytes for Ethernet). Larger IP packets need fragmentation or path MTU discovery.

**Q12. How does CRC help?**
Sender computes a checksum from frame contents and appends it; receiver recomputes. A mismatch means corruption; the frame is discarded.

---

## 15. Cheat Sheet

- MAC = 48-bit, hop-to-hop, first 24 bits vendor (OUI). Broadcast = `FF:FF:FF:FF:FF:FF`.
- Ethernet frame: Dest MAC | Src MAC | EtherType | Payload (max 1500) | FCS (CRC).
- Switch: learn → forward → flood → age.
- Hub: 1 collision domain; Switch: 1 per port; Router: separates broadcast domains.
- ARP: broadcast request, unicast reply, local network only.
- VLAN: logical segmentation, 802.1Q tags on trunks, needs L3 for inter-VLAN.
- STP: removes loops by blocking ports, elects a root bridge.
- CRC in Ethernet detects errors; it does not fix them.
