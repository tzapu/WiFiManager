/*
 * loader that handles template parameters in the form of {paramname}
 */
module.exports = function(content){
    let groups = JSON.parse(content.slice(content.indexOf("["), -1));
    // content, lang, name
    groups.forEach(group => {
        const re = /{([a-z]+)}/gm;
        group.groups = [];
        let match;
        while(match = re.exec(group.content)){
            group.groups.push(match[1]);
        }
        if(!group.groups.length){
            // no templates
            return;
        }
        group.content = group.content.split(/{[a-z]+}/);
    });
    return `module.exports = ${JSON.stringify(groups)};`;
};