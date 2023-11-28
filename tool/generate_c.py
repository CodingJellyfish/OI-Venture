import sys

if len(sys.argv) < 2:
    print("Please enter the file name as an argument")
    sys.exit(1)

filename = sys.argv[1]
author = "CodingJellyfish"
current_year = 2022
end_year = 2023

def snake_to_pascal_case(snake_str):
    components = snake_str.split('_')
    return ''.join(x.title() for x in components)

header = f"""/*  A competitive programming style dungeon game
 *  Copyright (C) {current_year}-{end_year} {author}
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU Affero General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Affero General Public License for more details.
 *
 *  You should have received a copy of the GNU Affero General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#define {filename.upper().replace("/", "_")}_C

#include "{filename}.h"

void Kh{snake_to_pascal_case(filename.replace("/", "_"))}Import(ecs_world_t *world)
{{
    ECS_MODULE(world, Kh{snake_to_pascal_case(filename.replace("/", "_"))});
}}

"""

with open("src/" + filename + ".c", "w") as f:
    f.write(header)