Simulated sync call to a contract

 - How to Build -
   - cd to 'build' directory
   - run the command 'cmake ..'
   - run the command 'make'

 - After build -
   - The built smart contract is under the 'caller' directory in the 'build' directory
   - You can then do a 'set contract' action with 'cleos' and point in to the './build/caller' directory

 - Additions to CMake should be done to the CMakeLists.txt in the './src' directory and not in the top level CMakeLists.txt
