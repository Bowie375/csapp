# Malloclab
---
## Files
* _malloclab-answers.tar_<br/>
    This tarball contains my own answers for this lab.<br/><br/>

* _malloclab-handout.tar_<br/>
    This tarball is the original handout bag, download it and start building your allocator.<br/><br/>

* _malloclab.pdf_<br/>
    The guidance file, make sure you have read it through before coding.<br/><br/>

* _README.md_<br/>
    This file.

## Tricky points
* Try diffrent combination of method, that is:
    * how should we manage empty list ?
        * implicit-list
        * explicit-list
        * segragate-list<br/><br/>
    * which empty block should we choose ? 
        * first-fit 
        * best-fit<br/><br/>
    * how should we insert empty block into list ?
        * add-to-front 
        * add-to-last
        * in-order<br/><br/>

    I have implemented corresponding methods in my answer tarball as reference.<br/><br/>

* there is nothing special in this lab, I think, it's only laborious.<br/><br/>

## Some words to say
This lab is torturous, I promise to you. I have spent days debugging, digging traces of those annoying mistakes from myriads of outputs. Even more, I have to optimize my vulnerable code over and over again to get full mark :).<br/><br/>

But it's a really strong sense of fulfillment when you actually make it. So I encourage everyone interested to have a try. Your skill will be greatly tested and improved during the experience.

