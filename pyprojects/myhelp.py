import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import numpy as np



def Subsection(symbol="=", length=64, text=None):
    line = symbol*length
    print(line)
    if text is not None:
        nend = len(text)
        nbeg = 0
        while nend > length-4:
            print( "".join( ("| ", text[nbeg:nbeg+length-4], " |") ) )
            nbeg += length-4
            nend -= length-4
        print( "".join( ("| ", text[nbeg:], ' '*(length-4-nend), " |") ) )
    print(line)



def Preview(df, cols_target=[], output_figs=False, output_prefix='preview'):
    Subsection(text=output_prefix+" head")
    print( df.head(16))

    Subsection(text=output_prefix+" shape")
    print( df.shape )

    Subsection(text=output_prefix+" info")
    print( df.info() )

    Subsection(text=output_prefix+" summary")
    print( df.describe(include='all') )

    if output_figs:
        n = df.shape[-1]
        fig, ax = plt.subplots(figsize=(n,n))
        corr = df.apply(lambda x: pd.factorize(x)[0]).corr()
        sns.heatmap(corr, vmax=0.8, cbar=True, annot=True, fmt='.2f' )
        fig.savefig(output_prefix+ "_heatmap.png")
    
        for c in df.columns:
            for t in cols_target:
                if df[t].dtype.name == 'category':
                    fig, ax = plt.subplots()
                    sns.histplot( data=df, x=c, hue=t, stat='count', multiple='layer' )
                    filename = "".join( (output_prefix, '_hist_', c.lower(), '_', t.lower(), '.png') )
                    fig.savefig(filename)
                else:
                    fig, ax = plt.subplots()
                    sns.scatterplot( data=df, x=c, y=t )
                    filename = "".join( (output_prefix, '_scatter_', c.lower(), '_', t.lower(), '.png') )
                    fig.savefig(filename)




def SetDatatype(df, cols_category):
    for c in cols_category:
        if c in df.columns:
            df[c] = df[c].astype('category')

    return df



def Learn(X, Y, function_list=[]):
    from sklearn.model_selection import train_test_split
    X_train, X_test, Y_train, Y_test = train_test_split(X, Y, train_size=0.7, random_state=7)
  
    results = {} 
    models = {}
    for func in function_list:
        s, m = func(X_train, Y_train, X_test, Y_test) 
        results.update( s )
        models.update( m )

    for name, model in models.items():
        results[name]['holdoff_score'] = model.score(X_test, Y_test)

    return results, models


