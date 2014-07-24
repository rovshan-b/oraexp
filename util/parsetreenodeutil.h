#ifndef PARSETREENODEUTIL_H
#define PARSETREENODEUTIL_H

class ParseTreeNodeUtil
{
public:
    //finds top level child of parentNode containing position
    template<typename T>
    static T* findChildForPosition(const T* parentNode, int position, bool topLevel)
    {
        if(position == -1 || (parentNode->children.count() == 0 && topLevel)){
            return 0;
        }

        if(topLevel && parentNode->children.count()==1 && parentNode->children[0]->containsPosition(position)){
            return parentNode->children[0];
        }

        int endIx = parentNode->children.count() - 1;

        int startIx = 0;
        int checkIx = (endIx-startIx) / 2;

        T *item = 0;

        const T* currentParent = parentNode;

        while(item == 0){
            item = currentParent->children[checkIx];

            if(item->containsPosition(position)){
                if(topLevel){
                    return item;
                }else{

                    if(item->children.count() == 0){
                        return item;
                    }

                    currentParent = item;

                    endIx = currentParent->children.count() - 1;
                    startIx = 0;
                    //checkIx = (endIx-startIx) / 2; //will be set below

                    item = 0;
                }
            }else if(position > item->getTokenInfo()->startPos){
                item = 0;
                startIx = checkIx + 1;
            }else if(position < item->getTokenInfo()->startPos){
                item = 0;
                endIx = checkIx - 1;
            }else{
                item = 0;
                Q_ASSERT(false);
                break;
            }

            checkIx = (endIx-startIx)/2 + startIx;

            if(checkIx < startIx || endIx < startIx){
                break;
            }
        }


        return item;
    }
};
#endif // PARSETREENODEUTIL_H
