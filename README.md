# music-stuffle-using-c
This project is designed for users who wish to efficiently manage and play their personal music  collections using a simple and lightweight command-line interface. It is ideal for individuals who prefer  functionality and speed over complex graphical interfaces.
The application allows users to organize, play, 
shuffle, and repeat songs directly from their local directories without the need for heavy media players. It 
also serves as a valuable educational tool for learning core programming concepts. 
• Designed for music enthusiasts who prefer a minimal, offline music manager. 
• Provides a command-line-based interface for managing local audio files. 
• Eliminates dependence on bulky GUI applications like iTunes or VLC. 
• Ideal for students learning data structures, especially linked lists. 
• Demonstrates node creation, traversal, and deletion in a real-world scenario. 
• Helps users explore file handling and directory operations using C libraries. 
• Introduces system calls for song playback and cross-platform functionality. 
• Suitable for developers experimenting with C system programming. 
• Runs efficiently on low-end or terminal-based systems. 
• Acts as both a practical utility and an educational resource for programming learners. 
1.2 Identification of Problem 
With the evolution of digital music, users have shifted to large media players such as Spotify, VLC, or  
iTunes. However, these applications are heavy, GUI-based, and resource intensive. They may not work  
efficiently on older systems or during programming demonstrations. 
The specific problems identified are: 
1. Lack of simple, lightweight, and open-source command-line music managers. 
2. Absence of playlist managers using fundamental data structures like linked lists. 
3. Difficulty for students to visualize how playlists can be built dynamically using memory-linked 
nodes. 
Hence, there is a need for a simple, educational, and cross-platform solution that demonstrates how 
linked lists can power a real-world music playlist manager. 
1.3 Identification of Tasks 
The main task is to design and implement a C-based Music Playlist Manager that: 
• Scans a given directory and lists all .mp3 or .wav songs. 
• Stores each song in a doubly linked list node, allowing forward and backward traversal. 
• Provides menu options to: 
o View all songs 
o Play current song 
o Move to next or previous track 
4| P a g e 
o Shuffle the playlist randomly 
o Enable repeat (one/all) 
• Utilizes system commands to play songs with the default OS media player. 
• Supports all major operating systems (Windows, Linux, macOS). 
