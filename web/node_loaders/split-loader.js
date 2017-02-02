/*
 * split HTML content and extract groups
 */
const splitGroup = /<!--\s* (\/)?(HTTP_[A-Z_]+)\s*-->/g;
module.exports = function(content){
    content = content.slice(content.indexOf("\""), content.lastIndexOf("\""));
    const groups = [];
    let match;
    let group;
    while(match = splitGroup.exec(content)){
        if(!match[1]){
            // new group started
            group = {
                name: match[2],
                start: match.index + match[0].length
            };
        }else{
            if(!group){
                throw new Error("closed group " + match[2] + "but was not opened");
            }
            if(match[2] !== group.name){
                throw new Error("expected to close group " + group.name + " but found " + match[2]);
            }
            groups.push({
                name: match[2],
                content: content.slice(group.start, match.index)
            });
            group = null;
        }
    }
    if(!groups.length){
        throw new Error("no groups found");
    }
    if(group){
        throw new Error("group not closed: " + group.name);
    }
    return "module.exports = " + JSON.stringify(groups) + ";";
};