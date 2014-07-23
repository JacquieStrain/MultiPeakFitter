
To get the best fit for the full function with no fixed parameters, very good initial guesses are needed. This is done by doing a series of subfits to determine the intial guesses for the parameters. The tree that determines which subfit to do next is below. Double lines indicate a fit convergence, and single lines indicate non-convergence.

								gaus+quad
								    //	\
						  		  // 	  	  \ 
								 //	   	    \
					full_fixedHtail		gaus+const
						//	\				 //	\
		 			      //  	  \ 	 	   		//      \ 
				  	    //	    \			      //	    \
					  //   full_fixedHtail_fixedQuad  \	
				 	//			     //	\		        \
			  	     full	     		   //  	  \ 			RETURN 0
			    	   //	   \			 //    	    \
       				 //  	     \ 	       // 	full_fixedHtail_fixedQuad w/new Htail value	
			 	//           \	     //	 	//	  		\
 PRINT RESULTS  		 \	   // 	      //		          \
				      		   \	 //		    //           		    \
						   full_fixedQuad				RETURN 0							  
							//	\
				 	      	      //  	  \
					   	    //    	    \			
						  full     	      \		
						//	\	        \
					      //	full_fixedQuadQuad
		PRINT RESULTS 		//		\	
	 						      //		  \
							    full		RETURN 0
							  //	  \
							//	    \
			  PRINT RESULTS 	   RETURN 0

The vector ‘branch’ (in the function ‘main’) is used to track where you are in the tree. 
Example: The first thing that is done is a fit to the peaks with a simple gaussian plus quadratic (only). Consider the case where it converges: branch[0] = true and the next fit is with the full peak-fitting function, but where the Htail parameters are fixed. 
If that fit converges, branch[1] = true and the next fit is with the full peak-fitting function and no fixed parameters. 
If it does not converge, branch[1] = false and the next fit is with the full peak-fitting function, but where the Htail parameters and fixed and the quad and slope term of the quad bkgd are fixed to zero. 