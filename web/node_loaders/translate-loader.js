/*
 * translate placeholders marked by [[text-to-translate]], update translation file
 */
const _ = require("lodash");
const trans = require("../translation.json");

function translate(group){
    let match;
    const re = /\[\[(.+?)]]/g;
    while(match = re.exec(group.content)){
        let translation;
        if(group.lang === "en"){
            translation = match[1];
        }else if(trans.html[match[1]]){
            translation = trans.html[match[1]][group.lang];
            if(!translation){
                console.log(`missing translation for '${match[1]}' (language: ${group.lang})`);
            }
        }else{
            console.log(`adding key for missing translation entry '${match[1]}`);
            trans.html[match[1]] = {};
            require("fs").writeFileSync(require("path").join(__dirname, "..", "translation.json"), JSON.stringify(
              trans, null, 2
            ));
        }

        if(!translation){
            group.content = group.content.replace(match[0], match[1]);
        }else{
            group.content = group.content.replace(match[0], translation);
        }
    }
    return group;
}

module.exports = function(content){
    let groups = JSON.parse(content.slice(content.indexOf("["), -1));
    const languages = Object.keys(trans.html).reduce((prev, cur) => _.union(prev, Object.keys(trans.html[cur])), []);
    const groupCount = groups.length;
    for(let i = 0; i < groupCount; i++){
        languages.forEach(lang => {
            const group = _.clone(groups[i]);
            group.lang = lang;
            groups.push(group);
        });
        groups[i].lang = "en";
    }
    groups = groups.map(translate);
    languages.push("en");
    Object.keys(trans.cpp).forEach(tname => {
        languages.forEach(lang => {
            groups.push({
                lang: lang,
                content: trans.cpp[tname][lang] || trans.cpp[tname].en,
                name: tname
            });
        });
    });
    return `module.exports = ${JSON.stringify(groups)};`;
};