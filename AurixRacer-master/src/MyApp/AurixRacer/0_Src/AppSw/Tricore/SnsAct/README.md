# Software Configuration  
### 1. Ostu Thresholding
> While in Line Scanning, We have to find difference road, and lane. So, We have to find proper threshold,   
> and the way to obtain the potimal threshold, Otsu's within class variance is used.   
> The smaller the sum of two differences multiplied by the weight, the greater the separation between the classes.
> Calculated by compressing data from 0 to 127, which occurs from total 0 to 4095.  
### 2. Pure Pursuit
> Pure pursuit is a method of geometrically obtaining the road to be driven.  
### 3. Linear Regression
> Linear Regression is a regression technique that models linear correlation between dependent variables and one or more independent variables.  
> The reason why we choice this technique is that, when we couldn't find the lane because of some reasons(hardware problems, internal problems, etc),
> The board have to know where the car have to go. Not perfect way to expectation, but we thought that the best way to find exact direction without data.
