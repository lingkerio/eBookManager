# eBookManager

## background

I believe that many programmers have a habit of reading e-books, and I have this habit. I have a lot of e-books on my computer, but their management I think it's too geeky to simply add or delete with the mouse. So I developed this project.

## Tools used

I have used the package manager vcpkg and the build system CMake. Moreover I have used the third-party libraries, nlohmann json, curlpp and oatpp (but I haven't actually used the latter two yet).

## function

1. Te basic store and delete ebook
2. When I stored the book, I actually left a backup copy of the book to give the user a chance to regret it (this is a reference to Linus' philosophy of designing git).

## Expected function

1. Users can download the books they want by Commandline, which will be realized by curlpp or call curl direct.
2. For books, users can customize labels to classify books, and books will be stored in different folders by tags after classification.

## last

I know that when you read this readme, you might think, WOW, isn't that what Calibre does? I know, but I'd like to say that I'd like to use it under the command line, but I don't need calibre as versatile as I currently do. Secondly, this is just my first project, it's just a practice, I'm just doing it with the idea of practicing what I've learned, and I know very well that this project isn't really great, as I wrote in the CMakeLists .txt file, this program is actually just named toy by me, it's just a toy for a little fun, that's it,
