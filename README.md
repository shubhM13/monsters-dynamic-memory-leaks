# monsters-dynamic-memory-leaks 
This project illustrates C's dynamic memory operations and checks for memory leaks via an interesting Monster Killing Problem. 
      
# Details     
Several small monster trainers have come to you for advice regarding expeditions they're planning into various regions.       
You are writing a program to estimate how many monsters they can expect to capture in each region.            
• You've got a Small Monster Index that tells you the name, type, and relative commonality of all the small      
monsters in question.     
     o (A monster’s absolute commonality is the same in each region. A monster’s relative commonality will     
change region by region as calculations are performed – we’ll show you how that works shortoy.)     
• You've also got an atlas that tells you about the relevant regions and which small monsters are present in them.    
• Each trainer tells you which regions they're visiting, and how many monsters they intend to capture per region.     
• To estimate the number of a given monster M a trainer will capture in a region R:     
    o Divide the relative population of M in R by R's total relative population.    
    o Multiply the result by the total number of captures the trainer intends per region.     
    o Round this result to the nearest integer. .5 rounds up, so you can use round() and its friends. Note    
    that this can result in a total slightly different than the trainer intended!     
        
# Example Input and Output
• Example Input
    
  8 monsters
      
  StAugustine Grass 12         
  Zoysia Grass 8      
  WholeWheat Bread 6    
  MultiGrain Bread 10   
  Rye Bread 10    
  Cinnamon Spice 5    
  Pepper Spice 10   
  Pumpkin Spice 30    
      
      
  3 regions   
        
  Rome    
  4 monsters    
  StAugustine     
  Zoysia    
  WholeWheat    
  Pepper    
      
  Helve          
  5 monsters    
  StAugustine   
  WholeWheat    
  MultiGrain      
  Rye   
  Cinnamon    
        
  Aria    
  5 monsters    
  Zoysia    
  MultiGrain    
  Cinnamon    
  Pepper    
  Pumpkin   
      
      
  3 Trainers    
      
  Alice   
  5 captures    
  2 regions   
  Rome    
  Aria      
          
  Bob   
  4 captures      
  3 regions   
  Rome    
  Helve   
  Aria    
      
  Carol   
  10 captures   
  1 region    
  Aria    
    
• Example Output    
    
  Alice   
  Rome    
  2 StAugustine   
  1 Zoysia    
  1 WholeWheat    
  1 Pepper    
  Aria    
  1 Zoysia    
  1 MultiGrain    
  1 Pepper    
  2 Pumpkin   
        
  Bob   
  Rome    
  1 StAugustine   
  1 Zoysia    
  1 WholeWheat    
  1 Pepper    
  Helve   
  1 StAugustine   
  1 WholeWheat    
  1 MultiGrain    
  1 Rye   
  Aria    
  1 Zoysia    
  1 MultiGrain    
  1 Pepper    
  2 Pumpkin   
          
  Carol   
  Aria    
  1 Zoysia    
  2 MultiGrain    
  1 Cinnamon    
  2 Pepper    
  5 Pumpkin   
      
          
![image](https://user-images.githubusercontent.com/43113962/93522019-7e4d9380-f94e-11ea-91b3-eb8867df833a.png)    
    
      
