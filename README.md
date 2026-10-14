# CS220 Midterm Project

Hugo (hhuan139) / Grant (ghur2)

| Task | Difficulty | Dependencies | Owner | Due date | Status |
|---|---:|---|---|---|---|
| Create CLI operation definitions, required argument counts checks and error codes. create mappings for every supported command to its required arguments and exit code | 2 | None | Hugo | Oct 14 | In progress |
| Test and finish PPM I/O, allocation, and cleanup functions. error handle image headers, binary pixel data, allocation failures, write fialures, and image cleanup | 2 | Starter structs and `read_ppm` | Grant | Oct 14 | In progress |
| Implement and test `crop`. check (inclusive) top left and (exclusive) bottom right coordinates, allocate the correct dimensions and copy the selected pixels | 3 | Starter image structs | Hugo | Oct 17 | Not started |
| Implement and test `grayscale` and `pixel_to_gray`. apply the NTSC color conversion formula and assign the converted value to all three channels | 2 | PPM image representation | Grant | Oct 17 | In progress |
| Implement and test `rotate_ccw`. create & allocate reversed dimensions and check that every source pixel corresponds to the correct CCW destination position | 3 | Starter image structs | Hugo | Oct 20 | Not started |
| Implement and test `blend`, including different image sizes. apply alpha where both images contain pixels and use black for areas outside both images | 3 | PPM I/O functions | Grant | Oct 20 | In progress |
| Implement and test `pointilism` with seed randomness. this should only affect exactly 3% of pixels using seed 1 and filled circles with radii randomly chosen from 1 through 5 | 4 | Starter image structs | Hugo | Oct 24 | Not started |
| Finish and validate Gaussian `blur`. generate an oddsized kernel from sigma and apply normalization on truncated edge neighborhoods. This task should process all rgb channels | 4 | PPM I/O functions | Grant | Oct 24 | In progress |
| Bring together all operations into `main`. test every command, check argument values and counts, print errors to `stderr`, return the required codes, and free resources | 4 | All image operations complete | Hugo | Oct 28 | Not started |
| Run all test comparisons, compilation checks, memory checks, and zip the submission files. Check for valgrind, git log, and do last minute double checks | 4 | Integrated program | Grant | Oct 28 | Not started |