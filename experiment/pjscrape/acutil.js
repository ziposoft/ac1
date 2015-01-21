
function isVar(vartype)
{
return !( vartype === 'undefined');
};
function phantom_exit(code) {
    setTimeout(function(){ phantom.exit(code); }, 0);
    phantom.onError = function(){};
    throw new Error('');
}




