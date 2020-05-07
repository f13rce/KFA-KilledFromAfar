# KFA-KilledFromAfar

The unhackable FPS Game - Proof of concept

# Introduction

What if there's a way to fully prevent hackers in a FPS game? No wallhacks, no aimbots, no ESPs... You name it. KFA is a proof of concept that explores the possibilities on how to prevent these hacks and making it an unhackable game.

# How does it work?

As explained on YouTube (https://youtu.be/13yAnp4MB8Q), The KFA server is built up to do everything for the client - including the rendering of the game. This prevents bad actors being able to work through the game memory to find "hidden" information such as the enemy's position. Without this data, bad actors have no way to exploit the game's mechanics.

# Pros and cons

Pros of this method:

- Client only needs a few MB of storage space
- There's no more need for a leeching or intrusive anti-cheat
- The game can be played on any device supporting RGB rendering
- Most game-related patches only requires a server update
- No more leaking of data through packet sniffing
- No more techniques required to hide lag
- The server can be functional for multiple games

Cons of this method:

- Requires more bandwidth and data is more time-critical
- The server requires more processing power, or even a GPU
- The server becomes a central point of failure - If the server lags, everyone lags
- Clients with a high ping, congested network or are suffering packet loss will have a terrible experience

# Compiling and running

You will need Visual Studio 2017 or later and a Leadwerks Game Engine license to compile this project.

This proof of concept is not intended for serious play. Its only function is to show that it is possible to create an unhackable FPS game.
