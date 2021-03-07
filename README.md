![C/C++ CI](https://github.com/lucas-engen/mdcrack/workflows/C/C++%20CI/badge.svg)
[![CodeFactor](https://www.codefactor.io/repository/github/lucas-engen/iron/badge)](https://www.codefactor.io/repository/github/lucas-engen/iron)

# Iron
* A simple password cracker. If you are looking for more complete solutions, take a look at [John the Ripper](https://github.com/openwall/john)

# Usage:
    ./iron -h <password hash> -d <dictionary file> -t <algorithm>

# Benchmark results
<table>
    <tr>
        <th><font color="cyan">OS</font></th>
        <th><font color="red">Memory (RAM)</font></th>
        <th><font color="green">CPU</font></th>
        <th><font color="lime">Passwords/second (average)</font></th>
        <th><font color="pink">Wordlist size</font></th>
        <th><font color="orange">Password length</font></th>
    </tr>
    <tr>
        <td style="text-align:center">Linux Debian 5.4.19 x86_64</td>
        <td style="text-align:center">8GB</td>
        <td style="text-align:center">Intel(R) Core(TM) i5-4590S CPU @ 3.00GHz</td>
        <td style="text-align:center">3182086</td>
        <td style="text-align:center">10 GB</td>
        <td style="text-align:center">8-9 chars</td>
    </tr>
</table>
